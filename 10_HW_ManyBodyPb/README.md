# HW 10: N-body simulation
# Mr. Patomporn Payoungkhamdee SCPY 6138171

## Theoretical relations
Consider classic gravitational force from Newtonian equation we would get

<p align="center">
<a href="https://www.codecogs.com/eqnedit.php?latex=\vec{a}_{\alpha,&space;i}&space;=&space;\sum_{\beta\neq&space;\alpha}&space;Gm_\beta&space;\frac{(\vec{r}_{\beta,i}&space;-&space;\vec{r}_{\alpha,&space;i})}{|\vec{r}_{\beta,i}&space;-&space;\vec{r}_{\alpha,&space;i}|^3}&space;\hspace{50pt}&space;(0)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\vec{a}_{\alpha,&space;i}&space;=&space;\sum_{\beta\neq&space;\alpha}&space;Gm_\beta&space;\frac{(\vec{r}_{\beta,i}&space;-&space;\vec{r}_{\alpha,&space;i})}{|\vec{r}_{\beta,i}&space;-&space;\vec{r}_{\alpha,&space;i}|^3}&space;\hspace{50pt}&space;(0)" title="\vec{a}_{\alpha, i} = \sum_{\beta\neq \alpha} Gm_\beta \frac{(\vec{r}_{\beta,i} - \vec{r}_{\alpha, i})}{|\vec{r}_{\beta,i} - \vec{r}_{\alpha, i}|^3} \hspace{50pt} (0)" /></a>
</p>

## Convention
* Set G = 1 
* Random initial position and velocity from 

## Leapfrog Integrations
Since Taylor's expansion correction is not accuracy enough for long dt which would be exploid, in this homework I would use more accurate approach called "Leapfrog Integrations". The equation relation governed by
<p align="center">
<a href="https://www.codecogs.com/eqnedit.php?latex=\vec{v}_{\alpha,&space;i&plus;1/2}&space;\leftarrow&space;\vec{v}_{\alpha,&space;i}&space;&plus;&space;\vec{a}_{\alpha,&space;i}\Delta&space;t/2&space;\hspace{50pt}&space;(1)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\vec{v}_{\alpha,&space;i&plus;1/2}&space;\leftarrow&space;\vec{v}_{\alpha,&space;i}&space;&plus;&space;\vec{a}_{\alpha,&space;i}\Delta&space;t/2&space;\hspace{50pt}&space;(1)" title="\vec{v}_{\alpha, i+1/2} \leftarrow \vec{v}_{\alpha, i} + \vec{a}_{\alpha, i}\Delta t/2 \hspace{50pt} (1)" /></a>
</p>

<p align="center">
<a href="https://www.codecogs.com/eqnedit.php?latex=\vec{r}_{\alpha,&space;i&plus;1}&space;\leftarrow&space;\vec{r}_{\alpha,&space;i}&space;&plus;&space;\vec{v}_{\alpha,&space;i&plus;1/2}\Delta&space;t\hspace{50pt}&space;(2)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\vec{r}_{\alpha,&space;i&plus;1}&space;\leftarrow&space;\vec{r}_{\alpha,&space;i}&space;&plus;&space;\vec{v}_{\alpha,&space;i&plus;1/2}\Delta&space;t\hspace{50pt}&space;(2)" title="\vec{r}_{\alpha, i+1} \leftarrow \vec{r}_{\alpha, i} + \vec{v}_{\alpha, i+1/2}\Delta t\hspace{50pt} (2)" /></a>
</p>

<p align="center">
<a href="https://www.codecogs.com/eqnedit.php?latex=\vec{v}_{\alpha,&space;i&plus;1}&space;\leftarrow&space;\vec{v}_{\alpha,&space;i&plus;1/2}&space;&plus;&space;\vec{a}_{\alpha,&space;i&plus;1}\Delta&space;t/2&space;\hspace{50pt}&space;(3)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\vec{v}_{\alpha,&space;i&plus;1}&space;\leftarrow&space;\vec{v}_{\alpha,&space;i&plus;1/2}&space;&plus;&space;\vec{a}_{\alpha,&space;i&plus;1}\Delta&space;t/2&space;\hspace{50pt}&space;(3)" title="\vec{v}_{\alpha, i+1} \leftarrow \vec{v}_{\alpha, i+1/2} + \vec{a}_{\alpha, i+1}\Delta t/2 \hspace{50pt} (3)" /></a>
</p>

## Algorithm
* There is "MPI_TYPE_STRUCT" for Construct and can use in process