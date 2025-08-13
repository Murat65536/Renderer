### You really shouldn't run this program. It has quite a few memory leaks at the moment.
<picture>
  <img src="https://github.com/user-attachments/assets/2e6fc71a-7027-41ec-bc97-9c3c26d0f6ad" />
</picture>

A terminal-based 3d rendering engine. Uses ANSI escape codes to color the text.

Adapted from TheBennyBox

This only works on Linux.

You must either be a part of the `input` group or run the program with `sudo` for keyboard controls to work.

You can check if you are in the `input` group like this:

`groups $USER`

You can make yourself part of the `input` group like this:

`sudo usermod -aG input $USER`

After you make yourself part of the `input` group, reboot before running the program.
