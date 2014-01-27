env = Environment(
   CPPPATH=Dir('src'),
   LIBPATH=Dir('build'),
   CXXFLAGS='-std=c++11 -Wall -g'
)

Export('env')

SConscript('src/SConscript', variant_dir='build', duplicate=0)
SConscript('test/SConscript', variant_dir='build_test', duplicate=0)

# vim:set ft=python sw=3:
