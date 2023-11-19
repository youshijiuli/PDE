import math
import latexify


@latexify.get_latex
def solve(a, b, c):
    return (-b + math.sqrt(b**2 - 4 * a * c)) / (2 * a)


print(solve)
# \mathrm{solve}(a, b, c) = \frac{-b + \sqrt{ b^{2} - 4 a c }}{2 a}


@latexify.get_latex
def sinc(x):
    if x == 0:
        return 1
    else:
        return math.sin(x) / x
print(sinc)
# \mathrm{sinc}(x) = \left\{ \begin{array}{ll} 1, & \mathrm{if} \ x = 0 \\ \frac{\sin x}{x}, & \mathrm{otherwise} \end{array} \right.


@latexify.get_latex
def ImplyYTM_D(P, D=8):
    return 3 + (100 - P) / D
    
print(ImplyYTM_D)


"""\mathrm{solve}(a, b, c) = \frac{-b + \sqrt{ b^{2} - 4 a c }}{2 a}
\mathrm{sinc}(x) = \left\{ \begin{array}{ll} 1, & \mathrm{if} \ x = 0 \\ \frac{\sin x}{x}, & \mathrm{otherwise} \end{array} \right.
\mathrm{ImplyYTM\_D}(P, D) = 3 + \frac{100 - P}{D}"""