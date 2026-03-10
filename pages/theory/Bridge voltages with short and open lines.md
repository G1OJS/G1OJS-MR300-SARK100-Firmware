---
layout: default
mathjax: true
title: "Bridge voltages with open and short lines"
permalink: /BridgeVoltagesWithOpenAndShortLines/
---
What are the bridge voltages $V_z$ and $V_a$ if the reflectometer is connected to a lossles open or short circuit transmission line?

Working in normalised units ($Z_0=1.0+j0.0$),

<div>
  <table style='border:none;'><tr><td style='border:none;'>$$V_a=\frac{1}{\vert Z+1\vert }$$</td></tr></table>
</div>
<div>
  <table style='border:none;'><tr><td style='border:none;'>$$V_z=\frac{\vert Z\vert }{\vert Z+1\vert }$$</td></tr></table>
</div>

For a short-circuited transmission line, the input impedance is
<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$Z=j\tan(\beta l)$$</td></tr></table>
</div>

So, $\vert Z \vert = \tan(\beta l)$, and 

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$\vert Z+1\vert = \vert 1 + j\tan(\beta l) \vert  $$</td></tr></table>
</div>

As $\tan$ is $\frac{\sin}{\cos}$:

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$\vert Z+1\vert = \vert 1 + j\frac{\sin(\beta l)}{\cos(\beta l)} \vert = \frac{1}{\cos(\beta l)} \vert \cos(\beta l) + j\sin(\beta l) \vert  $$</td></tr></table>
</div>

and we know that

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$\vert \cos(\beta l) + j\sin(\beta l) \vert = 1  $$</td></tr></table>
</div>

So

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$\vert Z+1\vert = \frac{1}{\cos(\beta l)}  $$</td></tr></table>
</div>

Hence, 

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$V_a = \frac{1}{\vert Z+1\vert } = \cos(\beta l)$$</td></tr></table>
</div>

and

<div class='equation'>
  <table style='border:none;'><tr><td style='border:none;'>$$V_z = \frac{\vert Z\vert }{\vert Z+1\vert } = \frac{\tan(\beta l)}{\cos(\beta l)} = \sin(\beta l)$$</td></tr></table>
</div>

</div>

<div  style='clear:both'></div>
