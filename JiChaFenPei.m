tic
% B=[0.318	0.444 	0.238	0.162
%     0.064	0.067 	0.122	0.094
%     0.11	0.081 	0.158	0.132
%     0.065	0.113 	0.152	0.042
%     0.107	0.082 	0.091	0.122
%     0.087	0.048 	0.044	0.081
%     0.063	0.040 	0.025	0.082
%     0.062	0.034 	0.015	0.119
%     0.086	0.057 	0.105	0.07
%     0.037	0.034 	0.05	0.096];
B=[0.318	0.444 	0.222	0.169
0.064	0.067 	0.087	0.098
0.11	0.081 	0.145	0.139
0.065	0.113 	0.135	0.044
0.107	0.082 	0.126	0.128
0.087	0.048 	0.097	0.085
0.063	0.040 	0.044	0.086
0.062	0.034 	0.019	0.077
0.086	0.057 	0.093	0.074
0.037	0.034 	0.031	0.1];

[WeightMatrix ,ResultMatrix] = FormMatrix(1,B);
[BetterResult,BetterWeight] = PlanRequire(ResultMatrix,WeightMatrix);
xlswrite('ResultWeight.xlsx',BetterWeight',2,'B2');
xlswrite('ResultWeight.xlsx',BetterResult,2,'I2');


% [WeightMatrix ,ResultMatrix] = FormMatrix(2,B);
% [BetterResult2,BetterWeight2] = PlanRequire(ResultMatrix,WeightMatrix);
% 
% xlswrite('ResultWeight.xlsx',BetterWeight2,'С�����2λ����','C2');
% xlswrite('ResultWeight.xlsx',BetterResult2,'С�����2λ����','I9');

toc
%%
function [WeightMatrix ,ResultMatrix] = FormMatrix(NumberAfterDecimal,B)
%FormMatrix �γ����п����Ծ���
%   �˴���ʾ��ϸ˵��
if NumberAfterDecimal==1
    LINE=linspace(0,1,11);
else
    LINE=linspace(0,1,101);
end
[a, b, c,d]=ndgrid(LINE,LINE,LINE,LINE);
WeightMatrix=[d(:) c(:) b(:) a(:) ];
%Ȩ��1-3����Ϊ0
[m,~]=find(WeightMatrix(:,1:3)==0);
WeightMatrix(m,:)=[];
WeightMatrix=WeightMatrix';
%Ȩ��֮�ͱ�Ϊ1
WeightMatrix(:,sum(WeightMatrix)~=1)=[];

ResultMatrix=round(370*B*WeightMatrix);
ResultMatrix(11,:)=sum(ResultMatrix);
end

function [BetterResult2,BetterWeight2] = PlanRequire(ResultMatrix,WeightMatrix)
%PlanRequire�滮����
%  �滮����һ�������� 
% ���� ��ɽ ��� ��ɽ �ϲ� �²� ������ ��ͷ�� �ȹ�ׯ ��Ӫ
ExpectedValue=[98;30;40;38;40;38;26;11;38;11];
ResultMatrix(12,:)=sum(abs(ResultMatrix(1:10,:)-ExpectedValue));
LogicalIndex=ResultMatrix(12,:)<=min(ResultMatrix(12,:));


%LogicalIndex=abs(ResultMatrix(3,:)-ResultMatrix(5,:))<=15 & abs(ResultMatrix(4,:)-ResultMatrix(6,:))<=15 & abs(ResultMatrix(4,:)-ResultMatrix(9,:))<=15 &ResultMatrix(7,:)<15;
% LogicalIndex=abs(ResultMatrix(4,:)-ResultMatrix(6,:))<=5;
% LogicalIndex=abs(ResultMatrix(4,:)-ResultMatrix(9,:))<=5;
% LogicalIndex=ResultMatrix(2,LogicalIndex)<=ResultMatrix(4,LogicalIndex);
% LogicalIndex=ResultMatrix(8,LogicalIndex)<=ResultMatrix(2,LogicalIndex);


BetterResult2=ResultMatrix(:,LogicalIndex);
BetterWeight2=WeightMatrix(:,LogicalIndex);

if isempty(BetterResult2)
    BetterResult2=0;
end
if isempty (BetterWeight2)
    BetterWeight2=0;
end
end