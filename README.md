# Vektorielle Hauptgleichungen der Weber-Kraft-Dynamik

## 1. Vektordefinitionen (Kartesische Koordinaten)

### Ortsvektor

\[
\vec{r} = \begin{pmatrix} x \\ y \\ z \end{pmatrix} = r \begin{pmatrix} \sin\theta\cos\phi \\ \sin\theta\sin\phi \\ \cos\theta \end{pmatrix}
\]

### Geschwindigkeitsvektor

\[
\vec{v} = \dot{\vec{r}} = \begin{pmatrix} \dot{x} \\ \dot{y} \\ \dot{z} \end{pmatrix} = \dot{r}\hat{r} + r\dot{\theta}\hat{\theta} + r\sin\theta\dot{\phi}\hat{\phi}
\]

### Beschleunigungsvektor

\[
\vec{a} = \ddot{\vec{r}} = \begin{pmatrix} \ddot{x} \\ \ddot{y} \\ \ddot{z} \end{pmatrix} = 
\left(\ddot{r} - r\dot{\theta}^2 - r\sin^2\theta\dot{\phi}^2\right)\hat{r} + \left(r\ddot{\theta} + 2\dot{r}\dot{\theta} - r\sin\theta\cos\theta\dot{\phi}^2\right)\hat{\theta} + \left(r\sin\theta\ddot{\phi} + 2\dot{r}\sin\theta\dot{\phi} + 2r\cos\theta\dot{\theta}\dot{\phi}\right)\hat{\phi}
\]

## 2. Weber-Kraft in Vektorform

### Weber-Kraft zwischen zwei Massen

\[
\vec{F}_{12} = -\frac{GMm}{|\vec{r}_1-\vec{r}_2|^3}\left(
1 - \frac{(\dot{\vec{r}}_1-\dot{\vec{r}}_2)\cdot(\vec{r}_1-\vec{r}_2)}{c^2|\vec{r}_1-\vec{r}_2|} + \frac{(\vec{r}_1-\vec{r}_2)\cdot(\ddot{\vec{r}}_1-\ddot{\vec{r}}_2)}{2c^2}
\right)(\vec{r}_1-\vec{r}_2)
\]

### Bewegungsgleichung für Masse m

\[
m\ddot{\vec{r}} = \sum_{i} -\frac{GM_i m}{|\vec{r}-\vec{r}_i|^3}\left(
1 - \frac{(\dot{\vec{r}}-\dot{\vec{r}}_i)\cdot(\vec{r}-\vec{r}_i)}{c^2|\vec{r}-\vec{r}_i|} + \frac{(\vec{r}-\vec{r}_i)\cdot(\ddot{\vec{r}}-\ddot{\vec{r}}_i)}{2c^2}
\right)(\vec{r}-\vec{r}_i)
\]

## 3. Lösungen in Vektorform

### Bahngleichung (xy-Ebene)

\[
\vec{r}(\phi) = \frac{a(1-e^2)}{1+e\cos(\kappa\phi)} \left[1 + \frac{3G^2M^2}{c^2h^4}\left(1+\frac{e^2}{2}+e\phi\sin(\kappa\phi)\right)\right] \begin{pmatrix} \cos\phi \\ \sin\phi \\ 0 \end{pmatrix}
\]

### Geschwindigkeitsfeld

\[
\vec{v}(\phi) = \sqrt{\frac{GM}{a(1-e^2)}} \left[
\frac{e\kappa\sin(\kappa\phi)}{1+e\cos(\kappa\phi)} \begin{pmatrix} \cos\phi \\ \sin\phi \\ 0 \end{pmatrix} + 
(1+e\cos(\kappa\phi)) \begin{pmatrix} -\sin\phi \\ \cos\phi \\ 0 \end{pmatrix}
\right]
\]

## 4. N-Körper-Systeme

### Beschleunigung des i-ten Körpers

\[
\ddot{\vec{r}}_i = -\sum_{j\neq i} \frac{GM_j}{|\vec{r}_{ij}|^3} \left(
1 - \frac{(\dot{\vec{r}}_{ij}\cdot\vec{r}_{ij})^2}{c^2|\vec{r}_{ij}|^2} + \frac{\vec{r}_{ij}\cdot\ddot{\vec{r}}_{ij}}{2c^2}
\right) \vec{r}_{ij}
\]

mit \(\vec{r}_{ij} = \vec{r}_i - \vec{r}_j = \begin{pmatrix} x_i-x_j \\ y_i-y_j \\ z_i-z_j \end{pmatrix}\)

### Radialkomponenten

\[
\dot{r}_{ij} = \frac{\vec{r}_{ij}\cdot\dot{\vec{r}}_{ij}}{|\vec{r}_{ij}|}, \quad
\ddot{r}_{ij} = \frac{|\dot{\vec{r}}_{ij}|^2 + \vec{r}_{ij}\cdot\ddot{\vec{r}}_{ij} - \dot{r}_{ij}^2}{|\vec{r}_{ij}|}
\]

## 5. Tensor-Notation (Indexschreibweise)

### Weber-Kraft (komponentenweise)

\[
F_i^k = -\sum_{j\neq i} \frac{GM_j m_i}{r_{ij}^3} \left(
1 - \frac{\dot{x}_{ij}^l \dot{x}_{ij}^l}{2c^2} + \frac{x_{ij}^l \ddot{x}_{ij}^l}{2c^2}
\right) x_{ij}^k
\]

wobei \( x_{ij}^k = x_i^k - x_j^k \) (k = 1,2,3 für x,y,z)

**Notationskonventionen:**

- Fettdruck: \(\mathbf{r} = \vec{r}\) (Vektoren)
- Punkt: \(\dot{\mathbf{r}} = d\mathbf{r}/dt\) (Zeitableitung)
- \(r = |\mathbf{r}| = \sqrt{x^2 + y^2 + z^2}\) (Betrag)
- \(\mathbf{a}\cdot\mathbf{b} = a^kb^k\) (Skalarprodukt, Einstein-Summation)

---

© 2025 Weber-Kraft als Alternative zur Allgemeinen Relativitätstheorie - Komplette Dokumentation
