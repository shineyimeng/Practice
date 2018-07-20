%%ProjectEuler017

% If the numbers 1 to 5 are written out in words: one, two, three, four, five, 
% then there are 3 + 3 + 5 + 4 + 4 = 19 letters used in total.
% 
% If all the numbers from 1 to 1000 (one thousand) inclusive were written out in words, 
% how many letters would be used?
%  
% NOTE: Do not count spaces or hyphens. For example, 342 (three hundred and forty-two) 
% contains 23 letters and 115 (one hundred and fifteen) contains 20 letters. 
% The use of "and" when writing out numbers is in compliance with British usage.

ONES = ["zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",...
        "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen",...
        "eighteen", "nineteen"];
TENS = ["", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"];


for index=1:20
    if index > 0 || index <=20
        To_English(index)=ONES(index);
    else
        if index >20 ||index <100
            To_English(index)=strcat(TENS(mod(index,10)),);
        end
    end
    
end

Total_Letter=sum(strlength(To_English));
disp(Total_Letter);
