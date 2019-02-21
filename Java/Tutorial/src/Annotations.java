import java.lang.annotation.Annotation;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

@Retention(RetentionPolicy.RUNTIME)
@interface UserInfo {
	int id() default -1;
	String firstName();
	String lastName();
	String username();
	String email();
}

@UserInfo(
	id = 55,
	firstName = "Michael",
	lastName = "Grieco",
	username = "michaelg20",
	email = "michaelgrieco27@gmail.com"
)
public class Annotations {
	public static void main(String[] args) {
		Class aClass = Annotations.class;
		Annotation[] annotations = aClass.getAnnotations();

		for(Annotation annotation : annotations){
		    if(annotation instanceof UserInfo){
		        UserInfo myAnnotation = (UserInfo) annotation;
		        System.out.println("id: " + myAnnotation.id());
		        System.out.println("firstName: " + myAnnotation.firstName());
		    }
		}
	}
}