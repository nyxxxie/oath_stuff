This is a simple implementation of the event-based HOTP algorithm (RFC4226) and
the time-based TOTP algorithm (RFC6238).  I was facinated by these algorithm
and their adoption by huge businesses and companies for one-time password
generation, so I decided to write my own implementation of them for fun!

References:
* https://en.wikipedia.org/wiki/HMAC-based_One-time_Password_Algorithm
* https://en.wikipedia.org/wiki/Time-based_One-time_Password_Algorithm
* https://github.com/seryl/oath-toolkit
* https://github.com/clibs/sha1
* https://github.com/kokke/tiny-AES128-C
* https://security.stackexchange.com/questions/35157/how-does-google-authenticator-work
