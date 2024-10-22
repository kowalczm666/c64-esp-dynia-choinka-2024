10 open 5,2,1,chr$(10)
20 def fn vic(p)=53248+p
5000 read x$: if x$<>"*" then goto 5000
5020 dim xp(3):dim yp(3)
5030 dim dx(3):dim dy(3)
5040 xp(1) = 50:yp(1) = 50
5050 xp(2) = 100:yp(2) = 100
5060 xp(3) = 150:yp(3) = 150
5070 dx(1)=1:dy(1)=4
5080 dx(2)=4:dy(2)=-1
5090 dx(3)=-4:dy(3)=-2
5095 poke fn vic(32),0:poke fn vic(33),0:rem 12
5100 rem sprajty
5110 poke 2040,13
5120 poke 2041,14
5122 poke 2042,15
5130 for y = 0 to 63:read a%:poke 832+y,a%:next
5160 for y = 0 to 63:read a%:poke 896+y,a%:next
5170 for y = 0 to 63:read a%:poke 960+y,a%:next
5190 poke fn vic(21),7:rem enable wsio
5200 poke fn vic(39),8:poke fn vic(40),1:poke fn vic(41),7:rem kolory
5210 poke fn vic(23),7: poke fn vic(29),7: rem ekspand
5400 rem statyczne grafiki
5410 print chr$(147)
5420 print chr$(155)+"helloween dynia interface  esp32 risc-v"
5430 print chr$(151)+"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
5433 print " "
5434 print " "
5435 print " "
5436 print chr$(154)+"hpcp packages monitor"
5438 print " "
5440 print "UCCCCCCCCCCCCCCCCCCCCCI"
5442 print " "
5444 print " "
5448 print " "
5450 print " "
5452 print " "
5454 print " "
5456 print " "
5458 print "JCCCCCCCCCCCCCCCCCCCCCK"
5460 print chr$(17)+chr$(17)+chr$(17)+chr$(17)
5462 print chr$(151)+"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
5464 print "kowalczm666 2024   dapik.devops@plus.pl"
5466 tx% = 0
5499 restore: rem wskazuj na dane led
5500 rem petla ekranowa
5600 print chr$(19): rem home
5602 r6$=r5$:r5$=r4$:r4$=r3$:r3$=r2$:r2$=r1$:r1$=r0$
5604 read r0$
5606 if r0$="*" then restore: goto 5604
5610 print chr$(17)+chr$(17)+chr$(17)+chr$(17)+chr$(17)+chr$(17)+chr$(17)
5628 print "B ";r0$;" B"
5630 print "B ";r1$;" B"
5632 print "B ";r2$;" B"
5634 print "B"+chr$(158)+">";r3$;chr$(154)+" B"
5636 print "B ";r4$;" B"
5638 print "B ";r5$;" B"
5640 print "B ";r6$;" B"
5660 print +chr$(17)+chr$(17)+chr$(17) + "tx:";tx%
5662 tx%=tx%+1
5670 print#5, r1$
5900 poke fn vic(0),xp(1): poke fn vic(1),yp(1)
5910 poke fn vic(2),xp(2): poke fn vic(3),yp(2)
5915 poke fn vic(4),xp(3): poke fn vic(5),yp(3)
5920 for n=1 to 3
5930 xp(n) = xp(n) + dx(n): yp(n) = yp(n) + dy(n)
5940 if xp(n)>250 or xp(n)<50 then dx(n)=-1*dx(n)
5950 if yp(n)>200 or yp(n)<50 then dy(n)=-1*dy(n)
5960 next n
5999 goto 5500
7999 rem dane do ledow
8000 data "^r000r000.200000000"
8010 data "^0r000r00.100000000"
8020 data "^00r000r0.080000000"
8030 data "^000r000r.040000000"
8040 data "^g000g000.020000000"
8050 data "^0g000g00.010000000"
8060 data "^00g000g0.008000000"
8070 data "^000g000g.004000000"
8080 data "^b000b000.002000000"
8090 data "^0b000b00.001000000"
8100 data "^00b000b0.0002aa000"
8110 data "^000b000b.000000155"
8120 data "^00000000.000000000"
8130 data "^rrrrrrrr.000000000"
8140 data "^00000000.000000000"
8150 data "^rrrrrrrr.000000000"
8160 data "^00000000.000000000"
8170 data "^rrrrrrrr.000000000"
8180 data "^00000000.000000000"
8190 data "^rrrrrrrr.0000003ff"
8200 data "^00000000.0000003ff"
8210 data "^rrrrrrrr.0000003ff"
8220 data "^00000000.0000003ff"
8230 data "^rr00rr00.200000000"
8240 data "^00rr00rr.100000000"
8250 data "^r00rr00r.080000000"
8260 data "^r00rr00r.040000000"
8270 data "^0rr00rr0.020000000"
8280 data "^0rr00rr0.010000000"
8290 data "^b000b000.008000000"
8300 data "^0g000g00.004000000"
8310 data "^b000b000.002000000"
8320 data "^0g000g00.001000000"
8330 data "^bbbb0000.200000000"
8340 data "^0000bbbb.100000000"
8350 data "^bbbb0000.080000000"
8350 data "^0000bbbb.040000000"
8360 data "^g0g0g0g0.020000000"
8370 data "^0g0g0g0g.010000000"
8380 data "^g0g0g0g0.008000000"
8390 data "^000b000b.004000000"
8400 data "^00b000b0.002000000"
8410 data "^000b000b.001000000"
8420 data "^r000b000.200000000"
8430 data "^b000r000.100000000"
8440 data "^r000b000.080000000"
8450 data "^b000r000.040000000"
8460 data "^00g00000.020000000"
8470 data "^000000g0.010000000"
8480 data "^00g00000.008000000"
8490 data "^000000g0.004000000"
8500 data "^00g00000.002000000"
8510 data "^000000g0.001000000"
8999 data "*"
9000 rem sprajty
61010 rem dynia
61011 data 0,60,0,0,24,0,0,24,0,0,254,0,3,255,240,31
61020 data 255,252,63,255,254,120,254,62,121,126,159,252,126,159,255,254
61030 data 63,255,255,255,255,219,255,255,255,255,255,255,255,249,222,31
61040 data 125,13,206,124,105,238,63,227,252,15,255,240,1,255,128,3
61050 rem duch
61060 data 1,192,0,7,248,0,15,255,0,15,255,128,30,127,128,25
61070 data 63,192,27,176,192,27,182,64,27,54,64,28,112,192,31,249
61080 data 224,31,255,224,31,255,224,31,255,224,31,255,224,31,255,240
61090 data 31,255,240,29,255,248,27,142,184,61,231,94,104,193,139,1
61100 rem czacha
61110 data 0,0,0,0,0,0,0,124,0,0,255,0,1,83,128,2
61120 data 239,128,3,254,192,7,56,192,7,24,192,7,158,192,3,255
61130 data 192,1,255,128,1,215,0,0,254,0,0,124,0,0,84,0
61140 data 0,0,0,0,170,0,0,254,0,0,124,0,0,0,0,1
