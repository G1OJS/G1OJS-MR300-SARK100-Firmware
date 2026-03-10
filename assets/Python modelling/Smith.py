import numpy as np
import matplotlib.pyplot as plt

# Define |Z| values and |Gamma| values
theta_g=np.linspace(-np.pi, np.pi, 360) # Full circle for phase of gamma
theta_z = np.linspace(-np.pi/2, np.pi/2, 180)  # R > 0 for phase of impedance (passive loads)
modZ_values = [0.1, 0.25, 0.5, 1.0, 2.0, 4.0, 10.0]  # Normalized |Z| values
gamma_values = [0.1, 0.25, 0.5, 0.75]  # |Gamma| values to plot

# Function to compute Gamma for |Z| and theta
def compute_gamma(modZ, theta):
    Z = modZ * np.exp(1j * theta)  # Polar form of Z
    gamma = (Z - 1) / (Z + 1)      # Reflection coefficient formula
    return gamma

# Plot the Smith chart with |Z| loci
fig, ax = plt.subplots(figsize=(8, 8))

# Plot |Z| loci
for modZ in modZ_values:
    gamma = compute_gamma(modZ, theta_z)
    ax.plot(gamma.real, gamma.imag, label=f"|Z|={modZ:.2f}")

# Plot |Gamma| circles
for gamma_mag in gamma_values:
    x_circle = gamma_mag * np.cos(theta_g)  # Real part
    y_circle = gamma_mag * np.sin(theta_g)  # Imaginary part
    ax.plot(x_circle, y_circle, linestyle='--', label=f"|Gamma|={gamma_mag:.2f}")

# Add Smith chart unit circle
smith_circle = np.exp(1j * theta_g)
ax.plot(smith_circle.real, smith_circle.imag, color='k', linestyle='-', linewidth=1, label="Smith Chart Boundary")

# Formatting
ax.set_xlim(-1.1, 1.1)
ax.set_ylim(-1.1, 1.1)
ax.set_aspect('equal')
ax.axhline(0, color='gray', linestyle='--', linewidth=0.5)
ax.axvline(0, color='gray', linestyle='--', linewidth=0.5)
ax.set_title("Smith Chart: |Z| Loci and |Gamma| Circles")
ax.set_xlabel("Real(Gamma)")
ax.set_ylabel("Imag(Gamma)")
ax.legend()
plt.grid(visible=True, linestyle='--', linewidth=0.5)
plt.show()
