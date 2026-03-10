---
layout: default
mathjax: true
title: "Issues with Calculating the Complex Load Impedance In Unbalanced RF Wheatstone Bridges"
permalink: /LoadImpCalcIssues/
---

Whilst it is relatively straightforward to calculate the complex load impedance from just the bridge voltage *magnitudes*, this is subject to a few imprecisions and uncertainties. Measuring the voltage magnitudes involves some kind of diode detector, and these are subject to nonlinearities which must be callibrated out, and it is diffuclt to determine the sign of X with certainty, making onward calculations subject to uncertainty. This last point makes it difficult to apply correction factors to remove the effects of stray capacitance for example. Whilst the mathematical independence of $V_r$ from $V_z$ and $V_a$ allows us to determine the complex load from the magnitude, errors in $V_r$ relative to $V_z$ and $V_a$ can again introduce uncertainties.

Whichever form of equation is used to calculate R (either direct from bridge voltages or going first via VSWR), the logical dependence is the same; $V_z$ and $V_a$ give us the magnitude of the impedance, and $V_r$ and $V_f$ give us the magnitude of the reflection coefficient. Anyone who knows about [Smith Charts](https://leleivre.com/rf_smith.html) will know that a circle placed around the chart centre represents a locus of constant magnitude of the reflection coefficient. Less commonly seen is the other piece of information we need; the locus of constant magnitude of impedance. It is where these two loci intersect that we find our solution(s) for complex $Z_l$. 

The familiar lines on the Smith Chart show the intersecting loci of constant real part and constant imaginary part of the complex impedance, but they don't immediately convey the magnitude of that impedance. 

<section id="visualiser">
To help with this, I wrote this visualiser, hosted on codepen. It shows the loci where the magnitude of the impedance and the magnitude of the reflaction coefficient are the same as that of the load impedance. You can click and drag (or touch and drag) the load impedance point and see how this changes those loci. Note that when the load is close to being purely resistive, any measurement errors resulting in a change to the relative radii of the two circles will produce a larger error in the load reactance than for load impedances away from the resistance axis. This seems to be a fundamental problem for this type of impedance measurement approach, tbc.

<p class="codepen" data-height="800" data-default-tab="result" data-slug-hash="pvzqyod" data-pen-title="ModZmodGamma" data-user="DrAlan" style="height: 300px; box-sizing: border-box; display: flex; align-items: center; justify-content: center; border: 2px solid; margin: 1em 0; padding: 1em;">
  <span>See the Pen <a href="https://codepen.io/DrAlan/pen/pvzqyod">
  ModZmodGamma</a> by Alan (<a href="https://codepen.io/DrAlan">@DrAlan</a>)
  on <a href="https://codepen.io">CodePen</a>.</span>
</p>
<script async src="https://public.codepenassets.com/embed/index.js"></script>
</section>

<p>The graphic is symmetric when reflected in the X axis, becaue there are, in general, two values of complex impedance satisfying $|\Gamma|=A$ and $|Z|=B$ that are distinguished only by changing the sign of X, which corresponds to reflection in the x axis of the chart.</p>

<p>If we added measurement errors to this diagram, and $|\Gamma|$ contained errors such that $|\Gamma| < \frac{|Z_l-Z_0|}{|Z_L+Z_0|}$ , the loci would not intersect at all. When this happens, it appears that $R > |Z|$ . This is a physical impossibility, which begs the question of how to present these numbers on a display value or on a chart, and if we then try to calculate X we will find that it takes an imaginary value because it becomes the square root of a negative quantity; another quandry for presentation. The easiest way around this is to (quietly!) enforce the condition that $R<|Z|$ and calculate X from the 'capped' value of R. This makes everything appear fine, but doesn't address the underlying issue.</p>

To summarise, the issues we have to address include:
- How to calibrate out the offsets and nonlinearities in the detectors for $V_f, V_r, V_z, V_a$ (each of which will be different and vary with frequency and load impedance)
- What to do when R appears to be greater than mod Z
- What to do about the uncertainty of the sign of X







