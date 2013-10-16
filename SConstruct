import os

# Check if doxygen plugin is installed
if not os.path.exists("site_scons/site_tools/doxygen"):
    os.popen("mkdir -p site_scons/site_tools")
    os.popen("cd site_scons/site_tools && hg clone https://bitbucket.org/russel/scons_doxygen doxygen")
    
env = Environment(tools = ["default", "doxygen"], toolpath = ['site_scons/site_tools/doxygen'])
env.Doxygen("Doxyfile")
SConscript('src/SConscript', variant_dir='build', duplicate=0)


