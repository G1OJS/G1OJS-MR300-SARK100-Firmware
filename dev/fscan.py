import serial
import datetime
import os

def startComms():
  global sark
  sark = serial.Serial(port='COM7',baudrate=57700,timeout=1)

def sendCmd(cmd):
  sark.write((cmd+'\r\n').encode('utf-8'))

def getResponse():
  respraw=sark.readline()
  resp=respraw.decode('utf-8').rstrip().split(',')
  return(resp)

def scanbr(fStart,fStop,fStep):
  kHz=fStart
  print('Set Sark for PC link')
  while(sark.readline()==b''):
    pass
  while(sark.readline()!=b'>>'):
    pass
  print('Serial link OK')
  sendCmd('scanbr '+str(fStart)+'000 '+str(fStop)+'000 '+str(fStep)+'000')
  resp=['']
  tstamp,kHz,mzuc, mzc, mguc, mgc = [],[],[],[],[],[]
  while(resp[0]!='Start'):
    resp=getResponse()
  while(resp[0]!='End'):
    resp=getResponse();
    print(resp)
    if((resp[0]!='End') & (resp[0]!='')):
      tstamp.append(datetime.datetime.now().strftime("%H:%M:%S"))
      kHz.append(float(resp[0]))
      mzuc.append(float(resp[1]))
      mguc.append(float(resp[2]))
      mzc.append(float(resp[3]))
      mgc.append(float(resp[4]))
  return kHz,tstamp,mzuc,mguc,mzc,mgc


def scanv(fStart,fStop,fStep):
  kHz=fStart
#  print('Set Sark for PC link')
#  while(sark.readline()==b''):
#    pass
#  while(sark.readline()!=b'>>'):
#    pass
  print('Serial link OK')
  sendCmd('scanv '+str(fStart)+'000 '+str(fStop)+'000 '+str(fStep)+'000')
  resp=['']
  tstamp,kHz,Vf,Vr,Vz,Va,gs = [],[],[],[],[],[],[]
  while(resp[0]!='Start'):
    resp=getResponse()
  while(resp[0]!='End'):
    resp=getResponse();
    print(resp)
    if((resp[0]!='End') & (resp[0]!='')):
      tstamp.append(datetime.datetime.now().strftime("%H:%M:%S"))
      kHz.append(float(resp[0]))
      Vf.append(float(resp[1]))
      Vr.append(float(resp[2]))
      Vz.append(float(resp[3]))
      Va.append(float(resp[4]))
      gs.append(int(resp[5]))
  return kHz,tstamp,Vf,Vr,Vz,Va,gs

def monitor_voltages():
    while(True):
      sendCmd('raw')
      print(getResponse())

def write_results(results_file, ID,kHz,tstamp,Vf,Vr,Vz,Va,gs):
    if not os.path.exists(data_path+'\\'+results_file):
       os.system('copy '+data_path+'\HEADER.csv ' + data_path+'\\'+results_file)
    Dt = datetime.datetime.now().strftime("%Y-%m-%d")
    fo=open(data_path+'\\'+results_file,'a')
    for i, f in enumerate(kHz):
        fo.write(str(1000*kHz[i])+','+ID+','+Dt+'-'+tstamp[i]+','+str(Vf[i])+','+str(Vr[i])+','+str(Vz[i])+','+str(Va[i])+','+str(gs[i])+'\n')
    fo.close();

def single_scan():
    ID=input("Measurement description? e.g. 'R_10'. note '_MR300_uc' will be added")
    ID=ID+'_MR300_uc'
    kHz,tstamp,Vf,Vr,Vz,Va,gs = scanv(1000,60000,233)
    write_results("NewMeas.csv", ID, kHz,tstamp,Vf,Vr,Vz,Va,gs)

def custom_repeat():
    archive_file="Analysis/Data/Custom.csv"
    print("Select R then press Enter")
    Rlist=[20,32,48,76,121,220]
    for R in Rlist:
        x=input(R)
        ID="R_"+str(R)+'_MR300_uc'
        kHz,tstamp,Vf,Vr,Vz,Va,gs = scanv(1000,60000,233)
        write_results("NewMeas.csv", ID, kHz,tstamp,Vf,Vr,Vz,Va,gs)

startComms()
data_path="Analysis/Data/"

#monitor_voltages()
#custom_repeat()
single_scan()







