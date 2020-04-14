#!/bin/sh

xtail /home/astro/.local/share/kstars/logs/* | grep --line-buffered -Fv "org.kde.kstars.indi] - ZWO CCD ASI120MM :  \"[INFO] Exposure done, downloading image... \"" | grep --line-buffered -Fv "Loading FITS file" | grep --line-buffered -Fv "Nothing to abort. " >> /home/astro/kstarslog.log
