import shutil
import subprocess

VariantDir('bin', 'src')

SetOption('num_jobs', 9)

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
# shutil.copy2('bin/sketchybar', '/Users/reeperto/.local/bin/')
