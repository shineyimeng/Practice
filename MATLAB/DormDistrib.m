for build=1:20
    clearvars -except build
    result=[];
    load matlab.mat
    %     build=17;
    
    for layer=1:6
        index=1;
        for no=1:50
            temp=find(building==build);
            temp=temp(dorm(temp)==layer*100+no);
            if(isempty(temp))
                continue;
            end
            result{8*layer-7,index}=layer*100+no;
            [m,~]=size(temp);
            for ti=1:m
                switch grade(temp(ti))
                    case 2013
                        result{8*layer-8+bed(temp(ti))+1,index}=[char(name(temp(ti))),'a'];
                    case 2014
                        result{8*layer-8+bed(temp(ti))+1,index}=[char(name(temp(ti))),'b'];
                    case 2015
                        result{8*layer-8+bed(temp(ti))+1,index}=[char(name(temp(ti))),'c'];
                    case 2016
                        result{8*layer-8+bed(temp(ti))+1,index}=[char(name(temp(ti))),'d'];
                end
            end
            index=index+1;
            clear temp
        end
    end
    if(isempty(result))
        continue;
    end
    xlswrite('result.xlsx',result,build);
end