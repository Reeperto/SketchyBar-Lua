import shutil
import os
import subprocess

VariantDir('bin', 'src')

cores = os.cpu_count()

if cores is not None:
    SetOption('num_jobs', cores + 1)
else:
    SetOption('num_jobs', 1)

env = Environment(
        CCFLAGS = '-std=c99 -g -Wall -Ofast -ffast-math -fvisibility=hidden -fno-common',
        COMPILATIONDB_USE_ABSPATH=True
        )

env.Tool('compilation_db')
env.CompilationDatabase()

env.AppendUnique(FRAMEWORKPATH='/System/Library/PrivateFrameworks')
env.AppendUnique(FRAMEWORKS = Split(
    'Carbon Cocoa CoreAudio CoreWLAN IOKit SkyLight DisplayServices MediaRemote'
    ))

prefix = subprocess.run(["brew --prefix"], shell = True, capture_output = True, text = True).stdout.strip()

env.ParseConfig(f"{prefix}/bin/pkg-config luajit --cflags --libs")

env.Program('bin/sketchybar', Glob('bin/*.c') + Glob('bin/*.m') + Glob('bin/lua/*.c'))
shutil.copy2('bin/sketchybar', '/Users/reeperto/.local/bin/')
