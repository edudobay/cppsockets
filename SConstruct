SConscript('colors.scons')
Import('env')

env = env.Clone(
   CPPPATH='src',
   CXXFLAGS='-std=c++11 -Wall')

lib_sources = ['src/' + fn + '.cpp' for fn in Split('EndPoint Error Poll Socket TCPSocket')]  

env.Program(target='bin/test', source=['test.cpp'] + lib_sources)
env.Program(target='bin/testsvr', source=['testsvr.cpp'] + lib_sources)

# vim:set ft=python sw=3:
