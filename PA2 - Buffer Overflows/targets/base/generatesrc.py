#!/usr/bin/python3

import click
import random
import re
import shutil
import sys

# Grab sid
sid = click.prompt('Enter your student ID').strip().upper()
sid2 = click.prompt('Confirm your student ID by entering it again').strip().upper()
if sid != sid2:
    print('Error: Student IDs don\'t match', file=sys.stderr)
    sys.exit(1)
if not re.match(r'^[AU][0-9]{8}$', sid):
    print('Error: Student ID is not in a recognized format')
    sys.exit(1)

random.seed(sid)

# Generate basic target
def gen_target(num,reps):
    ft = open("./base/target"+num+".c",'r')
    fto = open("target"+num+".c",'w')
    t = ft.readlines()
    ft.close()
    for l in t:
        for (v,t) in reps:
            l = l.replace(v,t)
        fto.write(l)
    fto.close()



# Setup target1
t1_replace = "T1BUFFER"
t1_val = str(128*random.randrange(1,7))

# Setup target2
t2_replace = "T2BUFFER"
t2_r = random.randrange(0,21)
if t2_r >= 13:
    t2_val = str(200+(t2_r-13))
else:
    t2_val = str(100+t2_r)

#Setup target3
t3_replace_1 = "T3_WIDGET_NUMDUBS"
t3_val_1 = str(2**random.randrange(2,4))
t3_replace_2 = "T3_MAXWIDGETS"
t3_val_2 = str(513+random.randrange(1,401))

# Setup target4
t4_replace_1 = "T4_BUFFER1"
t4_replace_2 = "T4_BUFFER2"
t4_val_1 = str(300 + 200*random.randrange(0,2))
t4_val_2 = str(300 + 25*random.randrange(0,7))

# Setup target5 
#t5_replace = "T5BUFFER"
#t5_val = "400"

# Generate targets
gen_target("1",[(t1_replace,t1_val)])
gen_target("2",[(t2_replace,t2_val)])
gen_target("3",[(t3_replace_1,t3_val_1), (t3_replace_2,t3_val_2)])
gen_target("4",[(t4_replace_1,t4_val_1), (t4_replace_2,t4_val_2)])
#gen_target("5",[(t5_replace,t5_val)])

