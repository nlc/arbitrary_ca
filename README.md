# arbitrary_ca
Generate Moore-neighborhood Cellular Automata with arbitrary rulesets.

Picks a random set of transition rules for each of the 2^(9) = 512 different configurations of the Moore neighborhood (including central square).

Many such rulesets are uninteresting but judicious selection of Hamming density, as well as some patience, can produce fairly interesting results. For example:

<img src="https://github.com/nlc/arbitrary_ca/raw/master/GoodOnes/ReallyGoodOnes/803C05A8C80A540C923186984909A026C122468438198200008948180614C61A3A600457CC508032A048012080480892541844208801A10895B6800815D40848.gif?raw=true"/>

This ruleset, ```803C05A8C80A540C923186984909A026C122468438198200008948180614C61A3A600457CC508032A048012080480892541844208801A10895B6800815D40848```, demonstrates multiple different types of ships and still-lifes, and puffers, and even shows a rudimentary ability for moving patterns to shift still-lifes without destroying them (e.g. look near the bottom right near the end of the animation).
