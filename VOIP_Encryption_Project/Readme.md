
# **VOIP With AES Ecnryption**

## **Problem**
The following are the problems with cellular telephone

-Cell phone calls are costly

-Special services are required for regional and international calls

-Not flexible to add many connections

## **Solution**

To overcome the above problems we go for IP based phone calls called VOIP  (Voive over IP). 
To make VOIP calls secure AES encryption is added.

## **Implementation**
Following are steps to implement VOIP call with Encryption

-Created a single UDP socket between client and server

-Threads are created in both server and client for sending and receving the data

-Recorded the voice using pulse audio

-Encrypted packets using AES technique rijndael-128 alogorithm(key is 128bits length) 

-Send the encoded packets to server (packet size = 10024)

-After receiving decrypt the packets at server

-Playback the voice using pulse audio


## **Gaps in Implementation**

Echo is audible when communicating

## **Challenges**

When fork is used in both server and client for sending and receving the data error encountered. So, multi threading is used.

Error : "child forked".

Encryption techniques like RSA is working for Text messages but when its applied to voice encrytion is not done.

## **Link**





