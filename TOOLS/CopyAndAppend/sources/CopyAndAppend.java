/**
 * User: Gunter
 * Date: 24.07.11
 * Time: 11:01
 */
public class CopyAndAppend {

    public static void main(String[] args) {

        CAndAAction caa = new CAndAAction();
        try {
            caa.checkArgs(args);
            caa.doCopyAndAppend();
        }
        catch (Exception e) {
            System.out.println(e.getMessage());
            CAndAAction.displayUsage();
        }
    }
}
