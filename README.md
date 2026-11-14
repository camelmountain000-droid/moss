# moss

**A terminal pet that grows moss on the friends you forget to text.**

Add people. Time passes. Moss grows. Gerald (the moss) has opinions.

    $ moss
    Miko          20d  """"""""""  getting mossy. a meme counts.
    Sara           3d  ""          a little fuzz. maybe say hi.

No guilt. Just a green nudge to say hi.

## Use it

    moss add Miko        add someone
    moss                 see who's mossiest
    moss ping Miko       you texted them, moss cleared
    moss share           a screenshot-ready report (no names in it)
    moss rm Miko         remove someone

## Install

Needs a C++ compiler (already on your Mac if you have git).

    git clone https://github.com/camelmountain000-droid/moss.git
    cd moss
    clang++ -std=c++17 -O2 main.cpp -o moss
    mkdir -p ~/.local/bin && cp moss ~/.local/bin/

## Privacy

Everything lives in one tiny file, `~/.moss_people`, on your own machine. No accounts, no internet, no tracking. `moss share` never prints names.

## Why

Out of sight shouldn't mean out of mind. About 150 lines of C++, zero dependencies, and one very judgmental plant.
