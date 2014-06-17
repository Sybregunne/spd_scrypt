from distutils.core import setup, Extension

spd_scrypt_module = Extension('spd_scrypt',
                               sources = ['scryptmodule.c',
                                          'scrypt.c'],
                               include_dirs=['.'], extra_compile_args=['-O3', '-msse3'])

setup (name = 'spd_scrypt',
       version = '1.0',
       description = 'Bindings for scrypt proof of work used by Speedcoin',
       ext_modules = [spd_scrypt_module])
