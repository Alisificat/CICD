#!/bin/bash
whoami
scp src/cat/s21_cat src/grep/s21_grep ubuntu5@192.168.195.52:/home/ubuntu5/
ssh ubuntu5@192.168.195.52 "mv /home/ubuntu5/s21_cat /home/ubuntu5/s21_grep /usr/local/bin/"
