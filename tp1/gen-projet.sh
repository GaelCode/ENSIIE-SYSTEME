# script pour créer l'arborescence projet utilisée
# dans les exercices avec find et tar

umask 022

DEST=/pub/ia/sys/shell/projet
#DEST=/tmp/prj

function dorep()
{
    d="$1"; bn=$(basename "$1")
    n="$2"
    e="$3"
    mkdir -p $DEST/"$d"
    while test $n -ne 0 ; do
        cat > "$DEST/$d/$bn$n$e" <<EOF
$d/$bn$n$e 
bla $n bla
bla $n bla
bla $n bla
EOF
    n=$[$n - 1]
    done
}

mkdir -p $DEST

################################################################
cat >  $DEST/README <<EOF
Bla Bla Bla
Bla Bla Bla
Bla Bla Bla
EOF

dorep demo 3
dorep doc  1  .pdf
dorep doc  4  .html
dorep doc/img  4 .png
dorep doc/img  4 .gif

################################################################
## src
mkdir -p $DEST/src/include
mkdir -p $DEST/src/util
mkdir -p $DEST/src/base

################################
## util
DI=$DEST/src/include
DS=$DEST/src/util
cat >  $DI/table.h <<-EOF
// fichier: table.h
// auteur:  Pierre Durant

typedef struct _Tcx {
    int x,y;
    char data[100];
} Tcx;
EOF
cat >  $DS/table.c <<-EOF
// fichier: table.c
// auteur:  Pierre Durant

#include "../include/table.h"

int tcx_cmpbonjour(Tcx * t, int i)
{
    return strcmp("bonjour",t[i].datat);
}
EOF
cat >  $DI/liste.h <<-EOF
// fichier: liste.h
// auteur:  Marcel dupont

Typedef struct _Tcxl {
    int x,y;
    char data[100];
    struct _Tcxl* next;
} Tcxl;
EOF
cat >  $DS/liste.c <<-EOF
// fichier: liste.c
// auteur:  Marcel dupont

#include "../include/liste.h"

Tcxl* tcxl_next(Tcxl * t)
{
    int z = atoi(t->data);
    return t->next;
}
EOF
cat >  $DS/Makefile <<-EOF
# fichier: Makefile
# auteur:  Marcel dupont

# strcmp atoi

all:
EOF
cat >  $DS/bug.c <<-EOF
// fichier: bug.c
// auteur:  jean Fleur

#include "../include/liste.h"

EOF
for i in {1000..9999} ; do
    echo "Tcxl* e$i;"
done >> $DS/bug.c
cp $DS/bug.c $DS/bug.c.save


################################
## base
DI=$DEST/src/include
DS=$DEST/src/base
cat >  $DI/data.h <<-EOF
// fichier: data.h
// auteur:  jean Fleur

typedef struct _Tdata {
    int x,y;
    char data[100];
} Tdata;
Tdata* dt_create(char *name); // utilise strcmp
EOF
cat >  $DS/data-in.c <<-EOF
// fichier: data-in.c
// auteur:  Patrice Burger

#include "../include/table.h"
#include "../include/data.h"

Tdata* dt_create(char *name)
{
    if ( strcmp("bonjour",name) == 0 )
        return dt_create_bonj();
    if ( strcmp("hello",name) == 0 )
        return dt_create_hello();
    return 0;
}
EOF
cat >  $DS/data-out.c <<-EOF
// fichier: data-out.c
// auteur:  Patrice Burger

#include "../include/table.h"
#include "../include/data.h"

void dt_print(FILE *stream, Tdata* dt)
{
}
EOF
cat >  $DS/Makefile <<-EOF
# fichier: Makefile
# auteur:  jean Fleur

# strcmp atoi

all:
EOF

################################
## src
DI=$DEST/src/include
DS=$DEST/src
cat >  $DI/main-util.h <<-EOF
// fichier: main-util.h
// auteur:  Patrice Burger

typedef struct _Targs {
    int x,y;
    char data[100];
} Targs;

Targs* main_cmdline(int argc,char*argv[]);

EOF
cat >  $DS/main-util.c <<-EOF
// fichier: main-util.c
// auteur:  Patrice Burger

#include "include/main-util.h"

Targs* main_cmdline(int argc,char*argv[]);
{
    int x = atoi[argv[1];
}
EOF
cat >  $DS/main.c <<-EOF
// fichier: main.c
// auteur:  Patrice Burger

#include "include/main-util.h"
#include "../include/table.h"
#include "../include/data.h"

int main(int argc,char*argv[])
{
}
EOF
cat >  $DS/Makefile <<-EOF
# fichier: Makefile
# auteur:  Patrice Burger

all:
EOF

