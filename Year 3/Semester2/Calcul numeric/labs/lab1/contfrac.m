function f=contfrac(n)
if n==1
    f=1+1/(1+1);
else
    f=1+1/contfrac(n-1);
end