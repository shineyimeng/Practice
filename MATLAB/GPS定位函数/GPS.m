%% GPS��λ
% ����
%%
% 
%  input.xls(��ʽ)
%  ns1:name	                    ns1:name2	ns1:description	ns1:color   ns1:width	ns1:color2	ns1:tessellate	����	  γ��	�뾶
%  OvitalMap_20180328_235845	Բ1	       Բ1��ע	        ffff0000	 2.00 	    7fff0000	1.00 	        117.59 	38.17 	0.50 
%  OvitalMap_20180328_235845	Բ2	       Բ2��ע	        ffff0000	 2.00 	    7fff0000	1.00 	        117.59 	38.17 	0.50 
% 



number=13;%���ݵ�ĸ��������Ը�����Ҫ�����޸�
theta=linspace(0,pi,number);
%% ��ȡ��һ������

format long
[~,~,raw]=xlsread('input.xls');%����matlab�Դ���excel��ȡ������ȡ������Ԫϸ��raw��

Longitude=raw{2,8};%����
Latitude=raw{2,9};%γ��
Radius=raw{2,10};%�뾶

%% �����һ������
%������data��
data1=zeros(3,number);
data1(1,:)=Longitude+Radius*cosd(theta)*0.01/0.873;
data1(2,:)=Latitude+Radius*sind(theta)*0.01/1.113;

%% �ڶ������ݴ���
Longitude=raw{3,8};%����
Latitude=raw{3,9};%γ��
Radius=raw{3,10};%�뾶
data2=zeros(3,number);
data2(1,:)=Longitude+Radius*cosd(theta)*0.01/0.873;
data2(2,:)=Latitude+Radius*sind(theta)*0.01/1.113;

%% �����һ������

fp=fopen('output.kml','w+');
fprintf(fp,'<?xml version="1.0" encoding="UTF-8"?>\n<kml xmlns="http://www.opengis.net/kml/2.2" \n 	xmlns:atom="http://www.w3.org/2005/Atom" \n	xmlns:gx="http://www.google.com/kml/ext/2.2" \n	>\n	<Document>\n		<name>');
fprintf(fp,'%s',raw{2,1});
fprintf(fp,'</name>\n		<Placemark>\n			<name>');
fprintf(fp,'%s',raw{2,2});
fprintf(fp,'</name>\n			<description>');
fprintf(fp,'%s',raw{2,3});
fprintf(fp,'</description>\n			<Style>\n				<LineStyle><color>');
fprintf(fp,'%s',raw{2,4});
fprintf(fp,'</color><width>');
fprintf(fp,'%d',raw{2,5});
fprintf(fp,'</width></LineStyle>\n				<PolyStyle><color>');
fprintf(fp,'%s',raw{2,6});
fprintf(fp,'</color></PolyStyle>\n			</Style>\n			<MultiGeometry>\n				<Polygon>\n					<tessellate>');
fprintf(fp,'%d',raw{2,7});
fprintf(fp,'</tessellate>\n					<outerBoundaryIs>\n						<LinearRing>\n							<coordinates>');

fprintf(fp,'%.8f ',data1(:));

%% ����ڶ�������
fprintf(fp,'</coordinates>\n						</LinearRing>\n					</outerBoundaryIs>\n				</Polygon>\n		</MultiGeometry>\n		</Placemark>\n		<Placemark>\n			<name>');
fprintf(fp,'%s',raw{3,2});
fprintf(fp,'</name>\n			<description>');
fprintf(fp,'%s',raw{3,3});
fprintf(fp,'</description>\n			<Style>\n				<LineStyle><color>');
fprintf(fp,'%s',raw{3,4});
fprintf(fp,'</color><width>');
fprintf(fp,'%d',raw{3,5});
fprintf(fp,'</width></LineStyle>\n				<PolyStyle><color>');
fprintf(fp,'%s',raw{3,6});
fprintf(fp,'</color></PolyStyle>\n			</Style>\n			<MultiGeometry>\n				<Polygon>\n					<tessellate>');
fprintf(fp,'%d',raw{3,7});
fprintf(fp,'</tessellate>\n					<outerBoundaryIs>\n						<LinearRing>\n							<coordinates>');

fprintf(fp,'%.8f ',data2(:));

fprintf(fp,'</coordinates>\n						</LinearRing>\n					</outerBoundaryIs>\n				</Polygon>\n			</MultiGeometry>\n		</Placemark>\n	</Document>\n</kml>\n');


fclose(fp);%�ر��ļ�