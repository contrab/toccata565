# toccata565

The goal of this program is to make a simple Arduino music player that plays an arbitrary-length melody without using the tone() call with the third argument (duration), which blocks all other processing while playing. This program achieves that, and also simultaneously blinks the board LED, to prove that other processing continues while the music plays.

The music is the opening bars of Bach's Bwv 565 Toccata in d minor. Why? Why not.

Note: the core playback functionality has been turned into an Arduino Library, found here: https://github.com/contrab/MelodyPlayer
