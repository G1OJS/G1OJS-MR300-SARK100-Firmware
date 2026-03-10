---
layout: default
mathjax: true
title: "Removing parallel impedances"
permalink: /RemovingParallelImpedances/
---

$$ Z_1=R_1+jX_1$$
$$ Z_2=R_2+jX_2$$

$$Z_1\//Z_2 = \frac{Z_1Z_2}{Z_1+Z_2} = \frac{(R_1+jX_1)(R_2+jX_2)}{(R_1+R_2)+j(X_1+X_2)} = \frac{R_1R_2-X_1X_2}{(R_1+R_2)+j(X_1+X_2)} + j  \frac{R_1X_2-R_2X_1}{(R_1+R_2)+j(X_1+X_2)}$$

Note that

$$\frac{1}{a+jb} = \frac{a-jb}{a^2+b^2}$$

So 

$$\frac{1}{(R_1+R_2)+j(X_1+X_2)}=\frac{(R_1+R_2)-j(X_1+X_2)}{(R_1+R_2)^2+(X_1+X_2)^2}$$

and

$$ Z_1\//Z_2 = \frac{(R_1R_2-X_1X_2 + j(R_1X_2-R_2X_1) )((R_1+R_2)-j(X_1+X_2)) }{(R_1+R_2)^2+(X_1+X_2)^2}$$

i.e.

$$Re(Z_1//Z_2) = \frac{R_1(R_2^2+X_2^2)+R_2(R_1^2+X_1^2)}{(R_1+R_2)^2+(X_1+X_2)^2}$$

and

$$Im(Z_1//Z_2) = \frac{X_1(X_2^2+R_2^2)+X_2(R_1^2+X_1^2)}{(R_1+R_2)^2+(X_1+X_2)^2}$$


For calculation, 

$$M^2_1=R_1^2+X_1^2$$
$$M^2_2=R_2^2+X_2^2$$
$$D=(R_1+R_2)^2+(X_1+X_2)^2$$
$$Re(Z)=\frac{R_1M^2_2+R_2M^2_1}{D}$$
$$Im(Z)=\frac{X_1M^2_2+X_2M^2_1}{D}$$




