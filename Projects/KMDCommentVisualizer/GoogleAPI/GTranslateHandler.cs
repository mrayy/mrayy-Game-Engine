using Google.Apis.Services;
using Google.Apis.Translate.v2;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoogleAPI
{
    public class GTranslateHandler
    {

        public void Init()
        {

            // Create the translation service.
         //   var service = new TranslateService();

         //   var response= service.Detections.List("Hello World").Execute();

          //  string lang= response.Detections[0][0].Language;
          //  Console.Write(lang);
        }
        public string DetectLanguage(string str)
        {
            return "";
        }

        public string TranslateLanguage(string str,string src,string dest)
        {
            return str;
        }
    }
}
