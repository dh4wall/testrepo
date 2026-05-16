public class es { 
    public static void sendm(){
        
        Messaging.SingleEmailMessage mail=new Messaging.SingleEmailMessage();
        mail.setToAddresses(new String[]{'dhawalshinde14@gmail.com'});
        mail.setSubject('this is another sample mail');
        mail.setPlainTextBody('hey this is a sample mail with a file so plz refer this');
        
        
            Messaging.EmailFileAttachment itt=new Messaging.EmailFileAttachment();
            itt.setFileName('hello.txt');
            itt.setBody(Blob.valueOf('this is a text file hope you recived it'));
            mail.setFileAttachments(new Messaging.EmailFileAttachment [] {itt});
        
        
        Messaging.sendEmailResult [] res= Messaging.sendEmail(new Messaging.SingleEmailMessage []{mail});
        
        System.debug('execution completed result:'+res[0].isSuccess());
        
        
        
    }
    
}

public class es {
    public String recv{get;set;}
    public String sub{get;set;}
    public String mbody{get;set;}
    public String finame{get;set;}
    public Blob body{get;set;}
    
    public void sendm(){
        if(recv==null || !recv.contains('@')){
            ApexPages.addMessage(new ApexPages.Message(ApexPages.Severity.ERROR,'invalid format mail'));
            return;
        }
        Messaging.SingleEmailMessage mail=new Messaging.SingleEmailMessage();
        mail.setToAddresses(new String[]{recv});
        mail.setSubject(sub);
        mail.setPlainTextBody(mbody);
        
        if(finame!=null && body!=null){
            Messaging.EmailFileAttachment itt=new Messaging.EmailFileAttachment();
            itt.setFileName(finame);
            itt.setBody(body);
            mail.setFileAttachments(new Messaging.EmailFileAttachment [] {itt});
        }
        
        Messaging.sendEmail(new Messaging.SingleEmailMessage []{mail});
        
        ApexPages.addMessage(new ApexPages.Message(ApexPages.Severity.CONFIRM,'mail sent'));
        
        
        
    }
    
}