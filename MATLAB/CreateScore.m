%% ���ɻ���꼶�ɼ�����-v0.1
% ����2013�����ɻ��Ա��Ϊ�˷��㹤������д�����matlab����������Ǹ������ɳ���
% ������ϣ�������ѧ��ѧ���Ǽǵ�Ŭ��ѧϰ�µķ������������Ľ�
% 
% �������������MATLAB2014b�����д�����ԣ�������������볢��2014b���Ժ�İ汾
% 
% % ���Ժ�ĸĽ���������ϵ��ʽ�Ա���ѯ
% v0.1���ߣ�¬����  QQ:531429414   MAIL:shine.yi@outlook.com
%% ׼��
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