
class Solution(object):
    def countBits(self, n):
        t=n
        ans=[0]
        for i  in range(1,n+1):
            t=i
            t2=0
            while t!=0:
                te=t%2
                if te==1:
                    t2=t2+1
                t//=2
            ans.append(t2)
        return ans

