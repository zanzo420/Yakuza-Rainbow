#pragma once
#include "DrawerSmall.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "..\Vectors.h"

Renderer* Renderer::m_pInstance;
extern "C" Vector3 WorldToScreen(Vector3 position);


Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
	const char yakuzalogo_compressed_data_base85[6645 + 1] =
		"7])#######H&Mo&'/###S),##1xL$#Q6>##e%1S:t@rT.=Zv%+2ej/1RZn426NqK>;.>>#ErEn/aNV=BgrD`%B*m<-eNE/1)1XGHgQ0^#J?uu#9XkV.d_(*HAs:T.KQZ`*'CRm/#-0%J"
		"W]W&-JLD21Drbf(C/d<BstSX'^r.>-b@pV-TT$=(lAwK%L?9C-VqEn/<_[FH$&`/#7*m<-PsEn/)2JuB7Vt*&eKwA-1rEn/%I[^IZ8QC%h3S>-#`e6/+>00Ft_9)MCN%)*D0r^#9%S+H"
		"(7TccNj5d*](-p/NcFVC9v>#PCvQ2MSXlGMEj@+12=_IoodPirp3=SnFtl&#<IPc_>iTj_Ih7S%:_h>,4h2W%WgJe$_^.R3k'f@u9Gg>&W0)X-;@DX:;7B.$/P,<%vwPV-5FL^#$,>>#"
		"-CkV74iWh#HtCfqeXZV$Ze5AOuQ'=(],LF%*op:#/v[fLaeVV$XQE1#I?<X(t:fq9>Uw?9g:U;-aRar?ASX%t[*@-dwOO&#$FSf.E2G>#VS1H-57YG-5ochLxqJfLw6aM-rs8gL3R()N"
		"<,+JM0<$##]5FGMk2#&#)aV&.e@=gLc6pfL(@E$#/Vs)#Fne+MO1$##8FUB#Tu$VmNji+D*:&1,%]tA#oO8u(Le?e?Mt#f$H&###q1m3#[Yc0-Px?v$#$o2_Z+Gb[M+GP8XNO&#s_`S%"
		"+e]&&H05##E,>>#?vG3#-Mc##3NAX-JgnO(r3xo%HBuo#C_Nb-##vEIR)l4SF`7T%HR(,)%^:D39Eo4Sc5V.q-.]lJ&0BYlc_L8.ACgV66v3,D>mKMTsh#/ho`buuvxrY5pLVA=Il9)E"
		"/G&/qSI$##plE.30n<:.*W8f3O29f3HMbL)nHUU)Z@[s$Wh#7*5$Il1)n?g)Yp?d),D,G4mrUv-A0SY-'.5R*6x?e6hZm3+0de8/^E`k+].I,*gY,H3/h'u$Yfum'tU>c4truLMActD#"
		"-1/LGcW[+4ol:[0('.O=H9ReM1QFjLtE4jLI`cHM#^3B-BW4?-BXvxLn$reOtn_d-2:#UVm^D.3/U[I-Y,ZpLC7YG-1Fi9.I6xt-.iqP'91Ie6mS[0l'fC0MT9MIMaltD#Cl$],jQ,@."
		"d]jj1qa9[9CgGT'Ap4[MS9emLi9Uv-hAMkF0dK)4u1SRM8iXI)A`fCNvp*+3@w4v6<5Ls-`HEjL=fXI)J%;m'LnE6svqEv6)d5[TG07IMf'<6.ws2FNEW#5MP&11M:h8)8XF1a4u`+XC"
		"]Q$+%d7=v-C<Tj2#_#`-HicERbsHR<9m#32x^t(MlqXI)GL5gLDw4D%:BJp7On-@'_9j;-]11%/s5Tv-b^O9`?rFsSbut0M6U2qLM@j?#C3tgl@:.W-.k;I-b>A,M9pWrMr)E.3di:a4"
		"vl>lLcD:68Pf,g)YLe5/=n-H3Z0ihLTT7Q-0?^8%Z#R'A3rH)4hq3R/9jE.3p&PA#rk[,*O.P?g=<Tv-E8/kX1I@1M?VL9M1P#<-Lvxn.Dk>A4.EOgMAUg=%J7$44e]OjL=wbP8Dg1T/"
		"7L@1MJ$uZ->Yx6*?K@T/oiE.3CUns-%Mko7bH>s.%-:w-P7PcMJ=hS-=5t;M^8Uv-9(hGM9)3iM3K8f32bN3M:*tV$/`#dMTH8f3&+#LGmQ:a#pXfC#Kv>U8[jK#$e>S'.dI+gL%A+jL"
		"ogt/M2EE.3a^D.3w/;o03X`=-b)9(/s<RF4aOQ^?lXZx6hv1=7sHi8.Di<<%?@i?#XpA88xAlD4O:-b[6v8+4ekcl$#=,O4fHgBJ#1d-?#u?1M?pls-;FseNOlQ1M'IXg$dt;R*Dlw],"
		"$,^;%PlS8/MfI*RSS9j1GsO?-I4r5/&i^F*MtTfLFMC).@7.1MH$&],&ZDa-#%Yt(opZ8/w`$]-h7X6NEtUhLW'Wm.PNXA#*c$n8?Oc)4j4p_4Fb*a4VMYS.MV&E##^#t$/EF,3T64a4"
		"<m[9MH:u$'qw;W-,wi0,En<:.jt9g25BGA#g2.^$F'r-MaFj?#aJ8f3S$]D4g__t(h8U$9/=h$')HRT0,iYW-,FUpTM:bN.t?(E#jaU9`bG+jLVvFc-)xZ#@ncdI%8#te3@mM&Z7RVX-"
		")Lq2`o[Me.@]WF3pLC).38cJMma0a#rqDs-9ch,N$ME49sf)I-Nkwv%<>Uv-e8tKPV*o8.au[v$v,aGMYPP,MaoB-%dL)H35GiU8pk0B#W7[x6:vBs-3.5GM^v;9/rtT8/?=Is-Q3378"
		"2wSe$1I$gLFHF/MMQ7IMCBrB#`:&Z$1h,O4?;^+4HjE.3L']u-pr:.N8^T%%:$&edoC-HNFhuL%'8M,*$<v-$KT,,2Og=W-bpEe-`AW8/T94a4:3eKY2dEgL-9<j$$vY59KE5f$WeAv6"
		"LM$=.KD,G4X8P)4MjEb7BN,[T25:m8S>Uk+_(mu%[79H)u]Ri(FvMx+8e7v3^gGR&UUtS&cs=NB&NB8%cYiS&TcJB+8#K],XplN'A=@W$=GDp/H:RW$Iqs5&PPjE*kW53'[>28&miB@6"
		"q2-,,*eOT%)$=D+@6[h(uZB2'N[[[#*F$>6^brv#JmoW7D[N5&Yo4?,0&Sh(+bl.)wutV-Z8?t$D<TU%[cW&+[NO9%X4Y$,)DOI)7xY29/:pm&#?-7&L#*a*PwNT%bHBU%HBUG)`s^%b"
		"l9jm/bm#N'h>;k'fNxw#%a>o/4vDo&qP1q%?%rv#1+p5'buS^+)b-u%R;fK):e&6&31$c*<JuCj^40+*VNcj'vb0J)LeR<$eWxW$UsB:%N2WA+7PAl'4YuY#-ZTZ-njUl'ZHKU%n;,X$"
		"Zqbs2/aEE*t$rh(auJF*Xgh,)>lx-)4qI],=x0+*b&)4'.`0X/g8bD+U=ik)WeMm/g.KQ&r$)%,x21E,72Rm0_#BbE(&nZ$N+(kLVQEv#WWCg(_W'U%SFsl&Tf&5'6(Z29?j?x-H6i-M"
		"uU`X.]JWa*[X.[#Hb39%M*o@#gl;k')SbW%?kxi'N3B6&>f+x>b?^2'm`lYulWbR:DlaX%+[Kq%TH.l`ewIK:2tTQ&a_Gc*S#.H)V+g$$r&T;.HA;P0)e0m&E0#N'tJ6*3KFd+>ZdV[%"
		"Xf-d)UK,N'JB1g(Ww=N1NK4?6TD>++QTlJ(A,[U9w]c>#JF<?#)Y4w51iCv#Dqjp%i.Fi(S9-ET<='.*U5Pq;h^9U%Ih39%K$^2'['1E#P:h@#$lJl'G``T/CeWT%FU<T%q7f1(E%M;$"
		"NVJ]$F+`;$D$BQ&eSuY#9.rv#-EwB+Kx1Z#@[<5&IewS%OK57&[f%+Mwgqc)iNg6&0/r11F$96&vs?5fJuhv#NiY>#EN_,)K4%k9Fr/:&Gc9T7[PQv$Od)7'Mg5;%mUp'+ggm.)2>VP0"
		";fgb*rh6t-BeWp%CaHT%5t-8'YOn=$o)0O9,)6K'#[j6/<xi=./L1*4<JBq@sKJJ1bNB[#R1lZ,/nEt$0g'N2,0.)*+r#%+Re3Y7;c+@5RJb1)GN@=)J=Dv#dI2'68Ni>5q0rVZ5=F=$"
		"mb#C+]XBK2]4B9&QA*&+ZgTm&Y%Ge#&@53'RP(c`6hEZ#6xCw,u9fY.6'TR/Y4p30u0wY#--D_$RD2<%ZgKm&HwE9%F*.k(F3,N'e3O?#NcP:.W2V0(`BYo&bs(k'Cv`]+&qwx,DqWt$"
		"f6N+ENH0:%Qb`S(]H5N'+VwZ$XlTE+C$96&GU<5&A<LG)(hla=QiZK1F,e*3vr[N:Pc:0*?=%w#u#ht8FB`?%]o*I)mK9Q&SK`S*uL06&*GlY#=%7s$3Y1Z#bX#c*3Tv/(-G=i(NeEp%"
		"fCmY#ER[s$OV.Q0%oI[#%(Fp9oQI&,87e8%C^_K(LPvs.KS38&Rt/q%feY?,e5iK(^1,(+Ok3Y-Y*tT%N@M;$I[R<$Ahe<$BU*T%dv,n&R_a5&?RA2'$d=:ADk#l1nseH)wh8m&DbWT%"
		"WsKU%3iU;$b2v7&?RIw#(5G>#qC&5'oHJj0LBL,)ZXh<-A=?:7SqS30pD`K(.W#<.J'7G*>eEt$gJnH)Y9k5&*cKf)tp9H2Ydu/:<r/e,nf[L(AH=>-3ZU:%EHcj'H3g6&nG)A$XhcnA"
		"Wx4$,CFsl&x^bT%_mKq%'26q7M&])*qa*I)%58L(t)R(>WXmiPZne<$JK,3'2Ze1(0+/@#Y467]ucP',B-^2'Y]hQ9L#HNToB0k%;ZMG0$0aA+OX%R%>Fv7+Urj,&`H(e,Q`l>#R/24'"
		";tj+3A5-X7FQ5O0ZHHN'&0?7&'5/$5)RcB$3UF#HKvqg(l:IW$YQt5&WdFW.[@<5&k,LU%XV***bbiV$e^+01OFn8%jK5k'saKY.SI.<$vpWu-#1O9%#//C4T0;5'8l_;$$*Bs$DlE<%"
		"BWbH;ac<I)N-N39['BQ&>i1d4^bwX-+B<T'-n@],B:7<$hM]/h&`$9.rgUg(JWrk'I_Ep%A^24'LJsa*XBn%5@JLC,4/[W%#voO9/rsp%Yt8:/+:n9.YLiZ#?Adb<e)J+4qOp+*[%Vv#"
		"*X60)9(iv#gSr4'RU4N1T^pE<lk?oDk4uh6YVP>#MfOv,N**d*CrM9/>'5N'_S;W%X%LM)rRl?,[l):%S5eH)$45A>+5,M(f^]301q'CG+3lt$[lwL(%-_q%$AEf2#ZEj;V[R<$Iae>6"
		"q*ef7-B9&&H'p2',_eN)QQhG)0[nT.pm1k'Nhgc31')O'^^B6&nj&V.ib/7&R/r0(gl$W$j4>6+FZSa+1:89%21*p%`Paa*CQQ:6HUwS%Bg`k'Bd6H)a&xV6N3>m)_RGG*hA/w%qHx#,"
		"Mo&p&14(J)>73p%<6UQ0LZlk00Fqv5/^v,,=:n8%63mK(ad/0:=.D;$;88h(BE@-)GEgU%`2&b4C,u?6d7)v#[3^Q&#w&7/Oud),wuNe)Ta#t$9w;w,^w8Q&=I[s$8XJ2'jN#Z.G.V;$"
		"IQQY+'v@Z56;:W-Vs?-)aJb1)T*X5&>ejp%jTjv#3c1?#947<$k/:H2;_NT%F6EZ#$->O031g`=Mxh8._TTQ&Dm&Q(2e)H*sM15066a]+JWcj'DKfA,:lS5'b40+*e23D>C*gI$@=nS%"
		"Ew/6&Y8v3'OBg2'^?sZ#G&nd)HRK*W^Aw)*<%U#,C%%W$:n6>6,x]G3`X(+*3sV4'ZgQY%n-2oKnD.h%kR'6&NKLg(o6Kl:TlAb*7(4WQ@c&R&m9cOKOSK)5$<Z3'r`H@$ju;s%P+M_,"
		"rIdA6Rvx*BwaaE*9H1'-]=84T^/2O'8L&B+26Ik(j8dK'PK>N'Y6g(0t+YZ,H%#q&WG0],<-CT9jGP>u5Qh0,Bfvp%5`E+3ID@],FI3p%L[ws$H>M/(g`s&+/`/p&k8(6&OTBY$.PcY#"
		"8@N5&F7D?#:$lf(GF@s$@+7<$/tI],V)qU%>gBI;:fc>#F=%[#H'g2'9`G>#BvYj'1kQo'>0uf(sh568k`cY#<O39%ni)s%R]sa*JfG>#E6Or.B*j914_V%,b(O=?i_i>7v#&pA,thA8"
		"/F%K:*nos$5Ia5&Qrs50-tfs$p]l>-d.Q>#Cq<9%Mbg=%qEFA#m[G<.ne4k'8oaX(FeX[$G-BQ&PuCv#pZwX-$4.X-h+Ah(Q[m%FDvC6&m+i:8DxNx-@Bl/(GK^L*w^oS%$9m/(w?R],"
		"-&*o&7OW5&DouY#s63208%RJ*DBqd<2dHW.X0oW$;.%s$[2HV%BORs$tUxp&MxTS.W9.#6e6%&,QmT#$E65n&r%3J2RxJX%19rhLRTWI+u]Z*3MwAm&4l1Z#B4>jBXaYN'Zfeh(ld0d2"
		"qQ(b=vK*Y-p?o@#]%XE*LVu>#]c7L(ikBa#o4c`+)7FT%4=WP&;t)d*k$#a=6g^'/-*mL(3`0b*^@'N(H.M;$J&@H)GuP>#fIe8%ZZ(K(TxD[,Ap2o&s=f1(J:r;$tdeA,s-TQ&4Y(Z#"
		".#5%PHh&6&IUiZ#,G[V%J2=+NwudV%m*cKLAF:N(L9gQ&n(wr%lCX.)NnB&$rwbm'+M1v#Dbmq&NpCK(qQKq%R;.L(?%I8%X2iO'nKXp%vK+9%qWbp%aTlj'wv06&ldB6&v^kp%&o#c*"
		"vF(<-nQ0)3b6Op%_K>3'S&Mg(WXI8%x7E50mH.a+Z08/L;9[B5r####O#.8##M#<-v@g;-<45dMTF6##fqg)NbL?##=FQ*NVRH##OU@/NWXQ##_m91N,@,gL@(o2Nc3*$#[x'/#+MC;$"
		"7F`t-K2f7NTF6##YUq9NUL?##_UE;NVRH##-o^)NWXQ##KGP-N,@,gL,Zc0N]w)$#R+T4Na?a$#3*-D-I*^%7jFebH0muhF]I#SC#*N=BQ_ijE(,DeG%GMeG'l-)O^V0,NWB940?6.L2"
		"=6[L25/(j,*;1eG38vLF*<eh2:NEI3p2fM1XJ*U8&dCEH/HE;I4n@F-vpNk1dZ].#3n@-#Y;L/#Ks:T.Tmk.#2H5s-^s6qLmKM/#GSrt-^c7NMvkP.#BA;=-KYwm/QB+.#a:F&#wE88R"
		"LHG&#I#<@0`VR]4+^>G25VLe$T`-F%5-lh6Rw6kO+7auG`W)aF0)uKG10`-6OW^f1R^sJ2S=*s`),]xKbq(eZdmv1Bd&4X:lXY'83Bk9;Sw+kb)d5si1.UiBULh%Fr+TV-T?*1#u<*1#"
		"rENvP&F5gL_gtA#(AcY#JrKS.m@TP8J>xXB^*B>#$,P:v2P###$&####m8At^;^$Y";

	const char yakuza_compressed_data_base85[6405 + 1] =
		"7])#######LREI`'/###S),##1xL$#Q6>##e%1S:t@rT.;H?D*8gj/1RZn42/.nP?L.>>#ErEn/aNV=B23R<(K*m<-uNE/1)1XGHgQ0^#J?uu#IXkV.d_(*HAs:T.JH?D*,BRm/#-0%J"
		"r2o^[9LD21[iFJ(X.d<BTd#=Txr.>-b@pV-TT$=(Y+.S%:?9C-VqEn/<_[FHb/r.$A(m<-PsEn/92JuBQT=Y)YLwA-ArEn/-I[^IDO.d#X0M>6l####+>00Fb,b0gJ6_c)Q5YY#_$S+H"
		"%&?lc@$iH-Y'-p/VcFVC9v>#P5KwA-.22X-14)qi^SlH3^)FTAeI1=#G<5)M9DP(j)-oeNoDp%ucTY^$5;Mt-Zr[fL0SafL:`O5Aivgr?DKP['k$/NM,]1EN+q:HMe0N?#_nBHM[F6##"
		"S(4GMoH<mL3I</1E#%S#dBP##]&Bi#=0v<-b/Vp.6.ac2iLSq)N]Lv#4=c%O9X@U.LFdH=lvhT0<Gbxufp3a$p7LhL2xSfLw7gV-RC2I$l7?>#]>SV-l`/r-XN^w'xOF&#.k%t%P^nd%"
		"'WfvLM/,*&$Un)&&Of;MS?f>-%AT;-6%se.*Ni,#+rls-s:OGMh(<$#wo8V-LJdp/P9F&#_AD0v(%)t-^_kgLla&NMYOHuuT6d7v,RNRM+eIfMPq%s$Awc-M_%vlMAu(T.(G:;$;FQS."
		"=R<T%(8N>mx.-N^jNvQ#^W,N$i&&'-^;DG)9`0H2;16Gi[BYg(:-jGiFSOV-NOMM0ne$v#*8###DctM(]ax9.7:tSnOQEMTU34^#QnA$0mP^2ut%Pq$k0I61$K6(#$_,3#I7G>#U%nM2"
		"+g^I*0n<:.[@i?#oNv)4`.v<-nn@:.=@[s$iGoU/l3rI3N#G:.__j=.co[:/KHaJM1fXI)?w?v$,4Z;%xkn?98HZ]4K%ImLiXZx6:1BI$,FYj1h%JD*%MbI)6plG*ElYW-BIO$KHg$&4"
		"'pXjLiikj1QtP<.af6<.wMQ_#RP4_.X8Qv$.t5D-t11lL^ot0Ml;dpLan+G4FnGk4a#+O4bL[?T>.#,MY*E.3Z*O,*5uAI$USI#-GpX?-jjO?-QuuS.#S`C,j`euL]Z7.ML%&f.*Gs`#"
		"pH-p-<Cv9i%@j?#>NjJM6=Uv-qqOSMjGrB#@.ikL_qd*MEF`hLW2J8%hLo+MY?*mLAi+G4K^'eZ6mN.3dP#H3A@%lL4XkD#9O?p-9&u?0]]d8/`]Wt%3,3D#(s5n&.O=5&ddg:%B&U<$"
		"4#f3(q.<w$ufU>$)3fY#_=nieC3Q/1H):R/Q-9Q&'H^W$tNJt$oR+&Hoe%w#ac@b#('r-MPSp%6EbmR&lp+9%ti*o7Z1;U8J<pm&,aaE*(SR9/6+g6*/cYM9)+Q'XMYfg%e[,G*vXVul"
		"3/%3'sC0H32sB+EUrvs.;gc#'H4%#Q?'uH.lQNaE4q9>$/t?k(Jfl%5pg5W.2]Zd+c'cJ(ut%t$'bO6&+4^2'dH0t8Gb<T%(Q3**BBM90<Mdt1cX0K,rWig2PR)o&_U9I)DrcV'QaLK("
		"77+<8#d$7&qO(e*J#gkr/e$WI'I6K)]Ia%=0+-&5HL,3'NWR^5N#[7&RTuj'Jx(Z#=i2v#Z#.H;XPF03]AA#,e_P(+mj0H2r@K6KoXEk:$.7x,uBK5:JHu/(4i^p7-Dh88`;*^.xh)?#"
		"`k-Y6$Pd&kHaoe*OP4v,W]Qp%'rmie)a.p%/<`r%pV8O(unrs$v'WW$jPHr%MXDv#eV/[%s8UH2%?T;.u`tW%iTm*4p9Hk(ecR'$.>h*%,%C-WmP`k't*tp%_+Le*[jb9%_l^3,UuPA6"
		"61R8%T5D0(/jZk*Yi&h?N[Na+suD5*VGhv-iUWQ&]Wl,OOBQM;jV8.,(Tj**gQ9<$)V't6m-6aN/r8U)cX;202tEv##Lx8%ifHF%wxQ]Q%q#n&)Y(=MZ7U3(SglN'UJv3^P.p^1Q#Rc,"
		"d%*G>MUEo&76V%50#gg2(?t)+52%+6C(xg;MS(Y.r#>^&']vt7TJ/)*JRE5&(MIv$c]@'$o2>40oHW;&wK-O(ncb;7M0Tm&FSi-2YZ(K(^1$p.@ZLU%B7%w#/l:$#YIXQ%,l%-#[^''#"
		"X[P+#YY$0#`pr4#_u4Y#dm9'#m;^Z-g@SfL,FYI)?]d8//Y<9/C]DD3CcK+*L?gmCG]XjL;v?A4IS)H*,x4gLcu;9/Tn5s.lYAgL@4Ms-ho1a4eZ'@0`KaR8X8.4=V5``3nT@lL=E=KM"
		"VH8f3fat9DKTNbNP,OV0Y>%&4<afF4jAgkLLJG,Ma61[-,cd3FGt*7N'0a;%jnNe-#5,<%iUYV-^uBj1Rl8nNrTZd3G[w`4v/&HM?c3?N?12iLT-,%MC3SRMMX7C#4EEb7Nm,b[dYbJM"
		"8Uq+$)x6S[s)KV.S7.-+=A5Z66j6MNgb>9%crj4U#JvE*Zp?e)c#oc*$IV@,DQ=>-:D,(j0(#6'BA_V.M(tk'@hlB$Vp@h(o_j9/u*.Y6$m,^,%9w`A+3pV.*U=p%UBO9'9hgX%$>$]6"
		"bXcI)7/Eb3>h#P1t,X[9`I$t-cLAw-/6jA+J*Q@XS6,fE_*eX-kSxw'LEkT%rSV0(sdtp%mek6'wgwE+Ow<p%H(oQ0v5YA##MA1(:Taj2*oPM93@CJ)v@>_+`M'j2c]f:-.bdG*+:m_+"
		"t4ta*q*M%,bTtp%]#RD*-@lm'uZ]N0qDm/(P,^>-uEoW$)T6I2(36r%OZln&Pp(4'cxj**SAnH)H5.8@i9'u.7goI*^#FL)eeTP'g%0'+Z`aS&BJq;.5kR],Su9DEH[Iw#?Z*)+UQpQ&"
		"Zx=Z,0.+A#EkZK)`VAB+AJ=M(GUOF)R^cN'=93D+Y_R8%YpnGVHYWb3/T&m(K6i/)DJP$,pETbaG6Tm&3I8@^a'RpKWE#T%cqqb**ERs?U$FP2MrVT/cT]X-3f7I+j8OE+LY74*;)XDs"
		")X*u-)E1117@M:@VvL0(jvYn&WXUf).0l9%_hL%,WO:J)xwtH)p0,%.]`nI2v#ur$v)Lf(cgrP8pvm+D$xn=c<A's-13av6=J))3q/l]#qSID*@F3/1lYWI)=@[s$YYmS/FG>8.OM>c4"
		">b$%'r=qP8Q=+o3%;:8.Ph>GMjAUv-W-M`N=Shp$*2g;%;8JB.BURw##>C?Rcf+G4$8-<-pG7^$DE*jLg0Ej9Pt/*#9htD#*/rv-nTV2idx<_$X5l8.v;Tv-0sig*qv&8%v61q`js4a4"
		"r&=kOJD5[pl**Q'QxE[8K2on3b+Hx6l1pb4n([8%oU5W)>jP=%p(0^+LX9:%mf-v$Mk*p%?u^19X%27:gxhV$i]-n&jaB6&=joa+oVZv$-442KTF-`m9X/9(*GNs%g],1:s[LsnJkNT%"
		"jHOp%BBY9<1nbrH-L:=YoJ-pqW5I>8]W-*3XFbk'`-=T%,V[Jp##''+qYl#7>PT$6)Z8i(1ikI)i_>c*WL1gCr2o;:P/E4'C6pm&O7^_4Fn<t$Plc:$bsV#9,srg(Ftsp%$/S1(ALYt9"
		"aiY`4d0r/)3P<k'4W%d#JRKD5j*Dj2sW-O'P096&:<?q9YcnSC)369%v(WP**-`4'VDo@Y)Ham/>`u_+5kMm5-+*aEb7N&F=w4k0Ln<9%[Etx#J4.G(Kdj#&Tfu;1gmqG)V^9U%YE#r%"
		"ef`?>lxEe)gK&C+Hi+B,Z.qS.Upu>><P=bP]hr?#RR?Z&gLCW-P]^u&ffrk'gb8%>^2`k'gHKs$'L+Z#-x.'(/M.nCK0oc;)xOi(F1xQ0^c%4Crhh'%XV3e)CwJQ&o:a<%e3UL23kU<H"
		"Vt/Q&dst]#xq,K)pk:@,4@qYI$bfk(^`EI)Mqjp%XS$m<=`.9%1<Wl(0f[$$Nio^+VA5D5haMlEX`b03Wxs50$)]h(r^NE*$kb3M^Dp0#iG^MulvgY$(A%%#Tm%P#D/CJ#Gugo.DOZa$"
		"CcK+*S7[x6rC^+4=W&J3KU%],r)b58,d5g)g-e39R]_$''Y@79-O1C&@-4W-/JwPhc4269OX;<%@f)T/TqJk=m06d30CILMW[v)4FmjI3m]MqLA%EY$xDgD4*2:u:2W]6E<$*nhL;$t."
		"6wPA4C/6ZeaitKM]3:pM?KYA#G_JH*m]WI)7HvTMt%^3OWr$T.]nr?#S^#pLu49w%0Ll;.Q56X1B`KP8ZGf'/ZIrM9x:a;74:]['joXjLOmQ1MuZZx6D'+W-)<>0lE7[=7_]s22@<oY-"
		"wV,eQo^D.3FI@1MjGl(N:hXI)'=e;-ES_lL`8n]4)v$>NJMihL0=tvL^t<_$/l,UT4I51%8_g2M@W.N%MHZ;.9sr;.2I@1Mm(.>%-_#V/<(,wp2R[LMV<r&M?,YsL*eCE%;GIT%e-^S%"
		"LEfx,eAG#-+uqW-gwes$@t3M(U8jA+uO'+*^TKq%9uhV$H3i3(%=-0)rD)O'5$,9%nUY$,2uP_+JtE9%Q%;v#T-OT%ux`o&<`NX%fe`m1+]Xi(ZcR.25h:0(dlo#,&m6Z$W$NU.M*1u."
		"5OYj'st%W.V?W/<[xq[eFK&],ZdPN'x3LR'j6sc+>^BZ-piwh(ppA;.UlJm'V(>Z,=15e)GguO0/j;o&5=e:.1cT+*hRAU/O8/^+iTLY$.AN0(74#^k;+06&hWtG2[c%%>k:Fm'-1>i("
		"(x*4:A+xS%[erv#]12g*W#%-)%:s20pAVK(x0rh(Snh=6r%PG4J*m:&BC7<$n<I12='T@#>vT;.gVUZ-W.M;$u[;ZGZ`sE*v&pR/:.m_+tT&n/]pgW$2I_/)9+4%$`d'Y$(xHt-)>#v,"
		"oosa*jB^Q&UufB+TX6)E]ij**q3830o@2Z#:,Fe)vosv%8(+N1vAmj'TdQ)*SSnr%I*X9%:m^r.n0X5&AbQO(R`L(,(b,R&w6Qc*sdOh'(:dC+',/l'gx%N^Y9k5&d`&'+X*tP&6l#(+"
		"lA)O'0i,G*Tvl3'-e8/1W:D?#eb,G*X,WA+8FM[,t-I9.?i4i(,#w3'.s:T/mfV4'iUG_+*cg#,Kg4L:-].R9kMJ[%J'tp%p=[tH7ckJ2@(lf(11>d3II9q%HQdM(r+fh(RkQv,lNjU."
		"[+iZ#L.Q,*NmGr%/H-*31lti(sYWa*X^w>68Oj5&]dP3'2$FO)u%81(D7e]#Ir3t?sAB5840[d)H0+H;jX,,*jG.1(+aKw@2]u>#N2`q%rq1@,=1%<$D9Nd*NT5n&juE.)G*q]$16n,*"
		"tL6++M_P`4Fr^b*#QDD3n4/m0Jqju-*vi0(7:?c*F4EX.9?O>7nUVv#;VJP'7D;t.iQxw#RCml/XjUK()=vC+xrwH)t&h)3UTHD*UK6b#Svc3'f]rk'=Uws$PjnL2jfxu,'.EU.:V[v$"
		"'Z111i%t**/b;d*;[Q,*Y>v7&[,d3'3=Ym'/W^v-fd6H)(AP##Y_0n%#$d+#=%T*#F#(/#V$###G.,Q'fh:8.X@[s$n&PA#mYWI)+2pb40@MP8S29k105%&4^7tM(8O%,;qvSe$W.Is$"
		"+P.3:sa(T/jdNw9ug'u$J&KHFgx=c4=LBu.J)jb&RwlS/jgZs.k&c>-r^j;-DtEa%`I8f3%IuD#_b:D+?Tf;Qp*gs$s:6kCf'$s.dkE1>5wln&Pm2EQqW&T%kD5(+lu]^+'X+K1<Z]Xu"
		"*';ruhSh[6]:h^+'E[(spQK6&B@Wa,JC^(4_G>Yucc(E4cArK(_bT1(l;)O'NL&%$>ntI#uWdp*35G>#v)KhTc=#_+>^w(+K[bc%el*i(Gl:v#cj+/)DZ9F%]#Z3'30/ZP5)KEurCnou"
		"?r`%,&oOgLNQN9'97<5&e).518US0>w6O7&<1E*,L[iw,S@Lt6<x'7'Z*2;&95ML1LE%=dG0(g(ZL2v#E`#xSj[hWD%no%#3.`$#u-G4%L]qr$+rbA#x[k&#,i68%C'`_&tI)20/ZJF."
		"YXJM'@Rj_&[efi'N'k_&8l4R*(Uk_&t>sx+2dxu#1:<p%:<Jp&2)m_&@w%F._Mm_&Gh/A=Lxm_&N>m92jpp_&kASY,fih_&j2sx+.oi_&4<mx4Jnj_&3+_%O+7V-MlYI;B5)#?Bc:@bH"
		"QWS]8rV'oD$g,-GXJu]5Xt%>-hHDiLqiob486%P1=?[L2L+%_>->hoDQ9eJ25fCkL=*8>-:=u41JKd<B,(rE-6vTSDSV+$/<rY1FAB1$p(4AVH'90F%TW7JCn#G_&=3G_&P/FP8;3;dE"
		"NuOcDK&f]GI,x&GSeDYG(;NY5jSb44G*w=P)<?p/7pm(#+E-(#UAg;-cxS,MmXr@-Dr$*.6sarLx(m.#@nVE-+j?x-Z&BkL%V.(#Q%nL-o:,G-MvGd.?n@-#&W3B-f8wx-16[qL9:SqL"
		"U(+f/`#S-#UaX.#tR0^#vC58..wx(3CkPJ$6;w<(VR-##(25##JQk&#o^''#tsTB#iM<H=l$2*H?*,##$)>>#2####$####(D:r$g/x.#";
	ImGuiIO& io = ImGui::GetIO();

	// This is my example of font initializing
	// First font used for UI
	// Second for rendering, 32.0f is pixel size, not font size.
	m_pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\l_10646.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesChinese());
	q_pFont = io.Fonts->AddFontFromMemoryCompressedBase85TTF(yakuza_compressed_data_base85, 20.0f);
	l_pFont = io.Fonts->AddFontFromMemoryCompressedBase85TTF(yakuzalogo_compressed_data_base85, 100.0f);

}

void Renderer::BeginScene()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("BUFFERNAME", reinterpret_cast<bool*>(true), ImVec2(0, 0), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
	ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	//window->DrawList->AddRectFilled(ImVec2(0, 0), ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGui::GetColorU32(ImVec4(.1f, .1f, .1f, 0.1f)));


}

void Renderer::DrawScene()
{
	ImGuiIO& io = ImGui::GetIO();
}

void Renderer::EndScene()
{

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->PushClipRectFullScreen();

	ImGui::End();
	ImGui::PopStyleColor();
}

float Renderer::DrawMyText(ImFont* pFont, PCHAR text, const Vector2& pos, float size, Vector4 RGB, bool center)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float r = RGB.x;
	float g = RGB.y;
	float b = RGB.z;
	float a = RGB.w;

	float y = 0.0f;
	int i = 0;

	ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text);

	if (center)
	{
		window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(.1f, .1f, .1f, a / 255)), text);
		window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(.1f, .1f, .1f, a / 255)), text);
		window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(.1f, .1f, .1f, a / 255)), text);
		window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(.1f, .1f, .1f, a / 255)), text);

		window->DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), text);
	}
	else
	{
		window->DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, a / 255)), text);
		window->DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, a / 255)), text);
		window->DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, a / 255)), text);
		window->DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, a / 255)), text);

		window->DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), text);
	}

	y = pos.y + textSize.y * (i + 1);
	i++;


	return y;
}

void Renderer::DrawLine(const Vector2& from, const Vector2& to, Vector4 RGB, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	window->DrawList->AddLine(ImVec2(from.x, from.y), ImVec2(to.x, to.y), ImGui::GetColorU32(ImVec4(RGB.x / 255, RGB.y / 255, RGB.z / 255, RGB.w / 255)), thickness);
}

void Renderer::DrawHealthbars(float PosX, float PosY, float height, float Value1, Vector3 color) {
	int HealthR = 0, HealthG = 0, HealthB = 0; // Lets make some integers so we can use them for the healthbar. What we will be using this for is to change the color of the healthbar according to the damage done to the enemy.

	float Value2 = static_cast<float>(Value1) / 100.f * height;
	int Value = static_cast<int>(Value2);

	if (Value1 > 0 && Value1 < 101) // if Value is greater then 75 and not greater then 90 draw the health has a darker shade of green.
	{
		DrawRect(PosX - 1, PosY - 1, 5, height, Vector4(0, 0, 0, 1));
		if (Value1 > 80) {
			DrawBoxFilled(Vector2(PosX, PosY), Vector2(PosX + 3, PosY + Value), Vector3(50, 205, 50), 0);
		}
		else if (Value1 > 60) {
			DrawBoxFilled(Vector2(PosX, PosY), Vector2(PosX + 3, PosY + Value), Vector3(255, 165, 0), 0);
		}
		else if (Value1 > 40) {
			DrawBoxFilled(Vector2(PosX, PosY), Vector2(PosX + 3, PosY + Value), Vector3(255, 165, 0), 0);
		}
		else if (Value1 < 40) {
			DrawBoxFilled(Vector2(PosX, PosY), Vector2(PosX + 3, PosY + Value), Vector3(220, 20, 60), 0);
		}
	}

}

void Renderer::DrawBox(const Vector2& from, const Vector2& size, Vector3 RGB, float rounding, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	//ImGuiContext* window = ImGui::GetCurrentContext();
	//window->OverlayDrawList.AddCircleFilled(ImVec2(400, 400), 30, 30, 12);
	window->DrawList->AddRect(ImVec2(from.x, from.y), ImVec2(size.x, size.y), ImGui::GetColorU32(ImVec4(RGB.x / 255, RGB.y / 255, RGB.z / 255, 255 / 255)), rounding, 15, thickness);
}

void Renderer::DrawBoxFilled(const Vector2& from, const Vector2& size, Vector3 color, float rounding, float opacity)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	window->DrawList->AddRectFilled(ImVec2(from.x, from.y), ImVec2(size.x, size.y), ImGui::GetColorU32(ImVec4(
		color.x / 255,
		color.y / 255,
		color.z / 255,
		opacity / 255)),
		rounding, 15);
}

void Renderer::DrawClearBox(const Vector2& from, const Vector2& size, DirectX::XMFLOAT4 color, float rounding)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	window->DrawList->AddRectFilled(ImVec2(from.x, from.y), ImVec2(size.x, size.y), ImGui::GetColorU32(ImVec4(color.x / 255, color.y / 255, color.z / 255, color.w / 255)), rounding, 15);
}


void Renderer::DrawBoxOutlined(const Vector2& from, const Vector2& size, Vector3 RGB, Vector3 OutlineRGB, float rounding, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	//ImGuiContext* window = ImGui::GetCurrentContext();
	//window->OverlayDrawList.AddCircleFilled(ImVec2(400, 400), 30, 30, 12);
	window->DrawList->AddRect(ImVec2(from.x, from.y), ImVec2(size.x, size.y), ImGui::GetColorU32(ImVec4(RGB.x / 255, RGB.y / 255, RGB.z / 255, 255 / 255)), rounding, 15, thickness);

	window->DrawList->AddRect(ImVec2(from.x, from.y), ImVec2(size.x, size.y), ImGui::GetColorU32(ImVec4(OutlineRGB.x / 255, OutlineRGB.y / 255, OutlineRGB.z / 255, 255 / 255)), rounding, 15, thickness * 1.2);
}

void Renderer::DrawCircle(const Vector2& from, float radius, DirectX::XMFLOAT4 color, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	window->DrawList->AddCircle(ImVec2(from.x, from.y), radius, ImGui::GetColorU32(ImVec4(color.x / 255, color.y / 255, color.z / 255, color.w / 255)), 180, thickness);
}

void Renderer::DrawCircleFilled(const Vector2& from, float radius, Vector4 color)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	window->DrawList->AddCircleFilled(ImVec2(from.x, from.y), radius, ImGui::GetColorU32(ImVec4(color.x / 255, color.y / 255, color.z / 255, color.w / 255)), 180);
}

void Renderer::DrawRect(float x, float y, float w, float h, Vector4 color)
{
	DrawLine(Vector2(x, y), Vector2(x + w, y), color, 1);
	DrawLine(Vector2(x, y), Vector2(x, y + h), color, 1);
	DrawLine(Vector2(x + w, y), Vector2(x + w, y + h), color, 1);
	DrawLine(Vector2(x, y + h), Vector2(x + w + 1, y + h), color, 1);
}

void Renderer::DrawOutlinedRect(float x, float y, float w, float h, Vector4 color)
{
	//DrawRect(x - 1, y - 1, w + 2, h + 2, Vector3(1, 1, 1));
	//DrawRect(x + 1, y + 1, w - 2, h - 2, Vector3(1, 1, 1));
	DrawRect(x, y, w, h, color);
}

void Renderer::DrawFilledRect(float x, float y, float w, float h, DirectX::XMFLOAT4 color)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	DrawClearBox(Vector2(x, y), Vector2(x + w, y + h), DirectX::XMFLOAT4(color), 0);
}

void Renderer::DrawBox3D(Vector3 positon, Vector4 color, float thickness, Vector3 min, Vector3 max, Vector3 angels)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();


	Vector3 corners[8] =
	{
		Vector3(min.x,min.y,min.z),
		Vector3(min.x,max.y,min.z),
		Vector3(max.x,max.y,min.z),
		Vector3(max.x,min.y,min.z),
		Vector3(min.x,min.y,max.z),
		Vector3(min.x,max.y,max.z),
		Vector3(max.x,max.y,max.z),
		Vector3(max.x,min.y,max.z)
	};

	Vector3 _corners[8];

	for (int i = 0; i <= 7; i++) {
		corners[i].Rotate2D(angels.x);
		corners[i].Rotate2D(angels.y);
	}

	for (int i = 0; i <= 7; i++)
		_corners[i] = WorldToScreen(positon + corners[i]);

	for (int i = 1; i <= 4; i++)
	{
		if (_corners[i].z >= 1.0f) {
			window->DrawList->AddLine(ImVec2(_corners[i - 1].x, _corners[i - 1].y), ImVec2(_corners[i % 4].x, _corners[i % 4].y), ImGui::GetColorU32(ImVec4(color.x / 255, color.y / 255, color.z / 255, color.w / 255)), thickness);
			window->DrawList->AddLine(ImVec2(_corners[i - 1].x, _corners[i - 1].y), ImVec2(_corners[i + 3].x, _corners[i + 3].y), ImGui::GetColorU32(ImVec4(color.x / 255, color.y / 255, color.z / 255, color.w / 255)), thickness); // ---
			window->DrawList->AddLine(ImVec2(_corners[i + 3].x, _corners[i + 3].y), ImVec2(_corners[i % 4 + 4].x, _corners[i % 4 + 4].y), ImGui::GetColorU32(ImVec4(color.x / 255, color.y / 255, color.z / 255, color.w / 255)), thickness);
		}
	}
}



Renderer* Renderer::GetInstance()
{
	if (!m_pInstance)
		m_pInstance = new Renderer();

	return m_pInstance;
}