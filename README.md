# Enhancing TCP Protocol with Partial Reliability using Sliding Window
This repository contains the C code for implementing the TCP protocol with partial reliability, featuring a sliding window and a tolerance mechanism for packet loss. The primary implementation is located in the "src/mictcp.c" file.

## Introduction

The goal of this project is to enhance the traditional TCP protocol with partial reliability. We achieve this by implementing a modified version of the TCP protocol named "mictcp". The modified protocol introduces a sliding window mechanism for managing the flow of packets and incorporates a tolerance mechanism to handle partial packet loss during transmission.


## TAGS:
- v1 -> Version 1 of mictcp
- v2 -> Version 2 of mictcp
- v3 -> Version 3 of mictcp
- v4 -> Version 4.1 of mictcp

## Running the Code:
- To compile run the command: `make`
- To use the Text Version, run this commands (in order) in two different machines or terminals:
   - Machine 1: `./tsock_texte -t mictcp -p`
   - Machine 2: `./tsock_texte -t mictcp -s`
- Video Version, run this commands (in order) in two different machines or terminals:
   - Machine 1: `./tsock_video -t mictcp -p`
   - Machine 2: `./tsock_video -t mictcp -s`
- (Note: Please use the "Wildlife" video instead of "Starwars" for video tests)
- You can adjust the partial reliability tolerance at the top of the "mictcp.c" file in the "src" folder.
- You can modify the loss rate in the "mic_tcp_socket" function at the top of the "mictcp.c" file in the "src" folder.

## Authors:
- Marti JIMENEZ
- Hugo GIRARD
