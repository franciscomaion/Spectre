data = np.loadtxt('/disk1/mjw/HOD_MockRun/Data/HODCube/WedgeMocks/Mock_40.dat')

pl.scatter(data[::10,0], data[::10,1])

pl.xlabel('x')
pl.ylabel('y')
pl.savefig('/disk1/mjw/HOD_MockRun/Plots/wedgeMocks_xy.pdf')
pl.clf()

pl.scatter(data[::10,1], data[::10,2])

pl.xlabel('y')
pl.ylabel('z')
pl.savefig('/disk1/mjw/HOD_MockRun/Plots/wedgeMocks_yz.pdf')

pl.clf()


MockCat    = np.loadtxt('/disk1/mjw/HOD_MockRun/Data/Del2k/midK_Del2k_Cube_Wedge_0_Jenkins1.0_kInterval_0.01_000.dat') 

MockCat    = MockCat[:,2]

for i in xrange(1, 40, 1):
    MockIn   = np.loadtxt('/disk1/mjw/HOD_MockRun/Data/Del2k/midK_Del2k_Cube_Wedge_'+str(i)+'_Jenkins1.0_kInterval_0.01_000.dat')
    MockCat  = np.vstack((MockIn[:,2], MockCat))

kvals        = MockIn[:,0]  

Mean         = np.mean(MockCat, axis=0)
Var          = np.var(MockCat,  axis=0)

pl.errorbar(kvals[1:]-0.0002, Mean[1:], np.sqrt(Var[1:])/np.sqrt(38), fmt='--')


MockCat    = np.loadtxt('/disk1/mjw/HOD_MockRun/Data/Del2k/midK_Del2k_Cube_Wedge_0_Clipped_Jenkins1.0_kInterval_0.01_000.dat') 

MockCat    = MockCat[:,2]

for i in xrange(1, 40, 1):
    MockIn   = np.loadtxt('/disk1/mjw/HOD_MockRun/Data/Del2k/midK_Del2k_Cube_Wedge_'+str(i)+'_Clipped_Jenkins1.0_kInterval_0.01_000.dat')
    MockCat  = np.vstack((MockIn[:,2], MockCat))

kvals        = MockIn[:,0]  

Mean         = np.mean(MockCat, axis=0)
Var          = np.var(MockCat,  axis=0)

A11          = 1.6 

pl.errorbar(kvals[1:]-0.0002, A11*Mean[1:], np.sqrt(Var[1:])/np.sqrt(38), fmt='--')


theory         = np.loadtxt('/disk1/mjw/HOD_MockRun/Data/HODTheoryPk/Pk_hod_20.0.dat')
pl.loglog(theory[:,0], theory[:,1], 'k--', label='vol. limited, $M_B< -20.0$')


linearBias     = 1.495903

theory         = np.loadtxt('/disk1/mjw/HOD_MockRun/Data/HODTheoryPk/camb_matterPk.dat')
pl.loglog(theory[:,0], linearBias*linearBias*theory[:,1], 'y--', label='linear')

pl.xlim(10**-2., 1.)
pl.ylim(100, 5*10**4)

pl.yscale('log')
pl.xscale('log')

pl.savefig('/disk1/mjw/HOD_MockRun/Plots/WedgeMocks_pk.pdf')
