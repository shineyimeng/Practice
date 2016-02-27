%% 自律会各年级成绩生成-v0.1
% 我是2013级自律会成员，为了方便工作所以写了许多matlab程序帮助我们更快的完成程序化
% 的任务。希望后面的学弟学妹们记得努力学习新的方法并且有所改进
% 
% 这个程序是我在MATLAB2014b上面编写的所以，如果遇到错误请尝试2014b及以后的版本
% 
% % 请以后的改进者留下联系方式以备咨询
% v0.1作者：卢熠猛  QQ:531429414   MAIL:shine.yi@outlook.com
%% 准备
[m,~]=size(stdorm);
result=zeros(m,21);
result(:,1)=stdorm(:,1);
for index=1:m
    for we=1:20
        temp=find(week==we);
        temp=temp(dorm(temp)==result(index,1));
        if(isempty(temp))
            continue;
        end
        result(index,we+1)=score(temp);
    end
end