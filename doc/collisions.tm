<TeXmacs|1.0.1>

<style|amsart>

<\body>
  Semi-realistic way of handling collisions, including rotations, without
  finding a point of collision. The goal is to find a small correction (the
  object acceleration) to the object velocities so that the overlap vanishes.
  Let the overlap area be approximated as

  <\equation>
    A(Q<rsub|0>+\<eta\>)=A<rsub|0>+\<eta\>\<cdot\>\<nabla\>A<rsub|0>
  </equation>

  where <with|mode|math|\<eta\>> is the correction to the velocities.
  Minimize the action

  <\equation>
    J(\<eta\>)=<big|sum><rsub|i><left|(>\<eta\><rsub|i>m<rsub|i><right|)><rsup|2><right|\|><rsub|f(\<eta\>)=A<rsub|0>+\<eta\>\<cdot\>\<nabla\>A<rsub|0>=0>
  </equation>

  by setting

  <\equation>
    \<nabla\>J(\<eta\>)=\<lambda\>\<nabla\>f(\<eta\>)\<Leftrightarrow\><left|{>2\<eta\><rsub|i>m<rsub|i>=\<lambda\>(\<nabla\>A<rsub|0>)<rsub|i><right|}>\<Rightarrow\>\<eta\><rsub|i>=<frac|\<lambda\>|2m<rsub|i>>(\<nabla\>A<rsub|0>)<rsub|i>
  </equation>

  which leads to the equation

  <\equation>
    <frac|\<lambda\>|2><big|sum><rsub|i><frac|(\<nabla\>A<rsub|0>)<rsub|i><rsup|2>|m<rsub|i>>=-A<rsub|0>\<Rightarrow\>\<lambda\>=<frac|-2A<rsub|0>|<big|sum><rsub|i>(\<nabla\>A<rsub|0>)<rsub|i><rsup|2>/m<rsub|i>>
  </equation>

  and finally

  <\equation>
    \<eta\><rsub|i>=-<frac|A<rsub|0>|<big|sum><rsub|j>(\<nabla\>A<rsub|0>)<rsup|2><rsub|j>/m<rsub|j>>\<cdot\><frac|(\<nabla\>A<rsub|0>)<rsub|i>|m<rsub|i>>
  </equation>

  \;

  With a collision normal <with|mode|math|n>, we require that the new
  velocities <with|mode|math|<wide|q<rsub|>|\<dot\>><rsub|i><rprime|'>>
  satisfy

  <\equation>
    <wide|q|\<dot\>><rprime|'>\<cdot\>n\<geqslant\>0
  </equation>

  \ by setting

  <\equation>
    <wide|q|\<dot\>><rprime|'>=<wide|q|\<dot\>>+\<alpha\>\<eta\><rsub|>\<Rightarrow\><wide|q|\<dot\>>\<cdot\>n+\<alpha\>\<eta\>\<cdot\>n\<geqslant\>0\<Rightarrow\>\<alpha\>=-e<frac|<wide|q|\<dot\>>\<cdot\>n|\<eta\>\<cdot\>n>,1\<leqslant\>e\<leqslant\>2
  </equation>

  where <with|mode|math|e=1> is a completely inelastic collision, and
  <with|mode|math|e=2> is a completely elastic collision.

  \;
</body>

<\initial>
  <\collection>
    <associate|paragraph width|150mm>
    <associate|odd page margin|30mm>
    <associate|page right margin|30mm>
    <associate|page top margin|30mm>
    <associate|reduction page right margin|25mm>
    <associate|page type|a4>
    <associate|reduction page bottom margin|15mm>
    <associate|even page margin|30mm>
    <associate|reduction page left margin|25mm>
    <associate|page bottom margin|30mm>
    <associate|reduction page top margin|15mm>
    <associate|language|english>
  </collection>
</initial>
