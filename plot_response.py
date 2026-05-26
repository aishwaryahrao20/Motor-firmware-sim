import re
import matplotlib.pyplot as plt

rpm_vals  = []
pwm_vals  = []
temp_vals = []

with open("step_response.log") as f:
    for line in f:
        rpm  = re.search(r'RPM=([\d.]+)', line)
        pwm  = re.search(r'PWM=([\d.]+)', line)
        temp = re.search(r'TEMP=([\d.]+)', line)
        if rpm and pwm and temp:
            rpm_vals.append(float(rpm.group(1)))
            pwm_vals.append(float(pwm.group(1)))
            temp_vals.append(float(temp.group(1)))

time = [i * 0.05 for i in range(len(rpm_vals))]  # 50ms telemetry period

fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(10, 8), sharex=True)
fig.suptitle('Motor Firmware Simulator — Step Response', fontsize=14, fontweight='bold')

ax1.plot(time, rpm_vals, 'b-', linewidth=2, label='Actual RPM')
ax1.axhline(y=1500, color='r', linestyle='--', linewidth=1.5, label='Setpoint (1500 RPM)')
ax1.set_ylabel('RPM')
ax1.legend()
ax1.grid(True, alpha=0.3)
ax1.set_ylim(0, 1700)

ax2.plot(time, pwm_vals, 'g-', linewidth=2, label='PWM Duty %')
ax2.set_ylabel('PWM Duty (%)')
ax2.legend()
ax2.grid(True, alpha=0.3)
ax2.set_ylim(0, 110)

ax3.plot(time, temp_vals, 'r-', linewidth=2, label='Motor Temp (°C)')
ax3.set_ylabel('Temperature (°C)')
ax3.set_xlabel('Time (s)')
ax3.legend()
ax3.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('step_response.png', dpi=150, bbox_inches='tight')
print("Plot saved as step_response.png")
plt.show()
