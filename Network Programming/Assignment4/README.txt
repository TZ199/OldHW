Team: Haoming Li (lih14) and Tianxin Zhou (zhout3)

Compile with "-lssl -lcrypto"

Usage: "./syncr server [port]" in the server directory"
       "./syncr client [port]" in the server directory"

Server responds to PUT, GET, QUERY, and CONTENTS
Client does the following: 
  ask for file list, CONTENTS
  for every file "yourfile" in the list
    if I dont have "yourfile", GET - non-overlapped
  for every file "myfile" I have
    if "myfile" not in list, PUT - non-overlapped
    else if "myfile" in list, QUERY
      if "myfile" newer, PUT - overlapped
      else if "myfile" older, GET - overlapped
