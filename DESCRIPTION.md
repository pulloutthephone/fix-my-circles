# Story

Circles are very interesting shapes. They transcend the obvious geometric
aspect and aid in much more complex problems. With the derivation of the
trigonometric functions, extraordinary things can be exploited by just using
circles to solve problems. You will probably bump into AC circuits if you read
a book about electrical circuit analysis. You may be introduced to sinusoidal
AC circuits while studying these circuits. A sinusoidal function, let us call
it $y(t)$, can be expressed as a circle moving in time (true story!):

$$ y(t) = A \sin(2 \pi f t + \phi) $$

> **_NOTE:_** This function is in the continuous domain.

If we take a step further and study signal processing, you may encounter the
Fourier's series and transform. Behind the hood, they work similarly; the main
idea is that any signal can be formed by a sum of sinusoidal signals with
different amplitudes, frequencies, and phases. And often, it's a *very* large
sum!

The Fourier transform utilizes this to **deconstruct** a signal in its
frequency domains. By analyzing the amplitude of each present frequency, it
reveals the signal's frequency spectrum. In mathematical terms, the transform
is defined as:

$$ F(\omega) = \int_{-\infty}^{\infty} f(t) e^{-i \omega t} dt$$

> **_NOTE:_** Using $i$ as the imaginary constant instead of $j$ irks me, but
for mathematical purposes, I'll keep it as such. ;)


On the other hand, the Fourier series performs the inverse operation: it
**constructs** a periodic signal by summing sinusoidal components with
specific characteristics. Its general form is:

$$ 
  f(t) = a_0 + \sum_{n=1}^{\infty} 
  \left[ a_n \cos\left(\frac{2\pi n t}{T}\right) +
  b_n \sin\left(\frac{2\pi n t}{T}\right) \right] 
$$



I could go on and on with more examples of circles used in different domains to
solve problems, but it is out of the scope for this Kata. In fact, if you’ve
read everything so far, here’s a lollipop :lollipop: — you just might be
serious!

Nonetheless, I think the point is made — circles are very interesting shapes! 

# Task
Someone has been corrupting my circles, and I need your help! If we do not fix
them, there will be a shortage, and people won't be able to use them to
solve problems anymore! :scream: 

Write a function `circle_mender` that takes as an input a string representing 
a circle with some holes and returns the same string with the holes filled.

Below are some examples to better represent what it is that is expected:

```
Example 1:
-------
Input:
                                        
                                        
                                        
                         #####          
                   #################    
                 #####           #####  
                ####               #### 
               ######            #######
                #######     ########### 
                 #####################  
                   #################    
                         #####          
                                        
                                        
                                        
                                        
                                        
                                        
                                        
                                        
-------
Output:
                                        
                                        
                                        
                         #####          
                   #################    
                 #####################  
                ####################### 
               #########################
                ####################### 
                 #####################  
                   #################    
                         #####          
                                        
                                        
                                        
                                        
                                        
                                        
                                        
                                        
```

```
Example 2:
-------
Input:
                                        
                                        
          #####                         
    #################                   
  #######     #########                 
 ######         ########                
#######           #######               
 ####               ####                
  #####           #####                 
    #################                   
          #####                         
                                        
                                        
                                        
                                        
                                        
                                        
                                        
                                        
                                        
-------
Output:
                                        
                                        
          #####                         
    #################                   
  #####################                 
 #######################                
#########################               
 #######################                
  #####################                 
    #################                   
          #####                         
                                        
                                        
                                        
                                        
                                        
                                        
                                        
                                        
                                        
```

```
Example 3:
-------
Input:
                                        
                                        
                                        
                                        
                                        
                                        
                                        
                 #####                  
         #####################          
      ###########################       
    ########     ##################     
  ########         ##################   
  ##########     ####     ###########   
 ##############             ##########  
  ###########             ###########   
  #############             #########   
    #################     #########     
      ###########################       
         #####################          
                 #####                  
-------
Output:
                                        
                                        
                                        
                                        
                                        
                                        
                                        
                 #####                  
         #####################          
      ###########################       
    ###############################     
  ###################################   
  ###################################   
 #####################################  
  ###################################   
  ###################################   
    ###############################     
      ###########################       
         #####################          
                 #####                  
```
