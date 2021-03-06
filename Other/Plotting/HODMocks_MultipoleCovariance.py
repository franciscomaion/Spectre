import copy
import scipy.interpolate

from   matplotlib.colors import LogNorm

cmap = copy.copy(matplotlib.cm.jet)

# cmap.set_bad('w',1.)

data = np.loadtxt('/disk1/mjw/HOD_MockRun/Data/Covariance/Clipped_HODCubeColumns_Covariance.dat')

plt.imshow(data, origin='lower', cmap='YlGnBu', interpolation='nearest', norm = LogNorm(vmin=10**0, vmax=3*10**9))

plt.colorbar()

pl.savefig('/disk1/mjw/HOD_MockRun/Plots/Multipole_Covariance.pdf', bbox_inches='tight', pad_inches=0.5)


pl.clf()

data = np.loadtxt('/disk1/mjw/HOD_MockRun/Data/Covariance/Clipped_HODCubeColumns_InverseCovariance.dat')
plt.imshow(data, origin='lower', cmap='YlGnBu', interpolation='nearest')
plt.colorbar()
pl.savefig('/disk1/mjw/HOD_MockRun/Plots/Multipole_InverseCovariance.pdf', bbox_inches='tight', pad_inches=0.5)


