file=["C:\Phoenix\Practice\党员应知应会题库.docx"];
try 
    Word=actxGetRunningServer('Word.Application');
catch
    Word=actxserver('Word.Application');
end
Word.Visible=1;

if exist(file,'file')
    Document=Word.Documents.Open(file);
else
    Document=Word.Documents.Add;
    Document.SaveAs(file);
end
Selection=Word.Selection;
Selection.Start=0;
a=[];
num=Documet.Range.end;
ii=0;
while ii<=num
    ii=ii+1;
    a=[a,Selection,text];
    Selection.MoveRight;
end
a=a(1:num);