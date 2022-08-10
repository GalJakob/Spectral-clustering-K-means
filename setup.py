from setuptools import Extension, setup

module = Extension("spkmeansmodule",
                  sources=[
                    'spkmeans.c',
                    'spkmeansmodule.c'
                  ])
setup(name='spkmeansmodule',
     version='1.0',
     description='Spectral Clustering',
     ext_modules=[module])
