from setuptools import setup, find_packages, Extension

# setup() parameters - https://packaging.python.org/guides/distributing-packages-using-setuptools/
setup(
    name='mykmeanssp',
    version='0.1.0',
    author="Gal Jakobwitz and Noa Barak",
    author_email="noabarak2@mail.tau.ac.il",
    description="kmeans project hw 2",
    install_requires=['invoke'],
    packages=find_packages(),
    license='GPL-2',
    ext_modules=[
        Extension(
            # the qualified name of the extension module to build
            'mykmeanssp',
            # the files to compile into our module relative to ``setup.py``
            ['kmeans.c'],
        ),
    ]
)