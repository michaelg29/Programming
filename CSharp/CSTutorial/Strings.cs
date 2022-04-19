using System;
using System.Text;

namespace CSTutorial
{
    public class Strings
    {
        public static void run()
        {
            string msg1;
            string msg2 = null;
            string msg3 = String.Empty;
            string path = "C:\\path";
            path = @"C:\path";
            string text = @"asdf
asdf
dfahgjdf";
            string quot = @"Her name was ""Sara.""";

            char[] letters = { 'A', 'B', 'C' };
            string alphabet = new string(letters);

            // string interpolation
            var jsonObj = (firstName: "Jupiter", lastName: "Hammon", born: 1711, published: 1761);
            msg1 = $"{jsonObj.firstName}";
            msg2 = $"";

            // raw literals - C#11
            /*
             *  """ """
             *  
             *  $$""" {{x}}"""
             */

            // accessing
            text = msg1.Substring(2, 4);
            text = text.Replace("j", "e");
            int idx = text.IndexOf("e");
            char c = text[text.Length - 1];
        }

        public static void sb()
        {
            // allows for modification in place
            var sb = new StringBuilder();
            for (int i = 0; i < 10; i++)
            {
                sb.Append(i.ToString());
            }
            sb[0] = sb[9];
        }
    }
}
