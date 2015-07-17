using Google.Apis.Auth.OAuth2;
using Google.Apis.Drive.v2;
using Google.Apis.Services;
using Google.Apis.Util.Store;
using Google.GData.Client;
using Google.GData.Spreadsheets;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace GoogleAPI
{
    class GoogleUser
    {

        static GoogleUser instance = new GoogleUser();
        public static GoogleUser Instance
        {
            get
            {
                return instance;
            }
        }

        GDataRequestFactory requestFactory;
        SpreadsheetsService service;

        public GDataRequestFactory RequestFactory
        {
            get
            {
                return requestFactory;
            }
        }
        public SpreadsheetsService Service
        {
            get
            {
                return service;
            }
        }


        public void Setup(string ID,string secret,string redirect)
        {
            string[] scopes = new string[] { DriveService.Scope.Drive,
                                 DriveService.Scope.DriveFile};
            OAuth2Parameters parameters = new OAuth2Parameters();

          // Set your OAuth 2.0 Client Id (which you can register at
          // https://code.google.com/apis/console).
          parameters.ClientId = ID;

          // Set your OAuth 2.0 Client Secret, which can be obtained at
          // https://code.google.com/apis/console.
          parameters.ClientSecret = secret;

          // Set your Redirect URI, which can be registered at
          // https://code.google.com/apis/console.
          parameters.RedirectUri = redirect;

          ////////////////////////////////////////////////////////////////////////////
          // STEP 3: Get the Authorization URL
          ////////////////////////////////////////////////////////////////////////////

          const string ServiceAccountEmail = "886476726924-fd7fo2lk1fkeegf3s6bcpv8aclics6vj@developer.gserviceaccount.com";

          var certificate = new X509Certificate2("Key.p12", "notasecret", X509KeyStorageFlags.Exportable);

          var serviceAccountCredentialInitializer =
              new ServiceAccountCredential.Initializer(ServiceAccountEmail)
              {
                  Scopes = new[] { "https://spreadsheets.google.com/feeds", "https://docs.google.com/feeds" }
              }.FromCertificate(certificate);

          var credential = new ServiceAccountCredential(serviceAccountCredentialInitializer);

          if (!credential.RequestAccessTokenAsync(System.Threading.CancellationToken.None).Result)
              throw new InvalidOperationException("Access token request failed.");

           requestFactory = new GDataRequestFactory("KDMChannel");
          requestFactory.CustomHeaders.Add("Authorization: Bearer " + credential.Token.AccessToken);

          service = new SpreadsheetsService("SpreadSheet-finder");
          service.RequestFactory = GoogleUser.Instance.RequestFactory;

        }
    }
}
