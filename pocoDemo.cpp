// #include <Poco/Net/SocketAddress.h>
// #include <iostream>
// using Poco::Net::DNS;
// using Poco::Net::IPAddress;
// using Poco::Net::HostEntry;

// int main(int argc, char** argv)
// {
// 	const HostEntry& entry = DNS::hostByName("www.appinf.com");
// 	std::cout << "Canonical Name: " << entry.name() << std::endl;
// 	const HostEntry::AliasList& aliases = entry.aliases();
// 	HostEntry::AliasList::const_iterator it = aliases.begin();
// 	for(; it != aliases.end(); ++it)
// 		std::cout << "Alias: " << *it << std::endl;
// 	// const HostEntry::AddressList& addrs = entry.addresses();
// 	// HostEntry::AddressList::const_iterator it2 = addrs.begin();
// 	// for (; it2 != aliases.end(); ++it2)
// 	// 	std::cout << "Address: " << it2->toString() << std::endl;
// 	return 0;
// }

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/Context.h"
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/ConsoleCertificateHandler.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/URI.h>
#include "Poco/StreamCopier.h"
#include "Poco/SharedPtr.h"
#include <Poco/JSON/Object.h>
#include <iostream>

using namespace Poco;
using namespace Poco::Net;

int main(int argc, char** argv)
{
  // SharedPtr<PrivateKeyPassphraseHandler> pConsoleHandler = new KeyConsoleHandler(false);
  // SharedPtr<InvalidCertificateHandler> pInvalidCertHandler = new ConsoleCertificateHandler(false);
  Context::Ptr pContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
  SSLManager::instance().initializeClient(0, 0, pContext);
  // SocketAddress sa("192.158.199.135", 8888);
  // StreamSocket socket(sa);
  // SocketStream str(socket);
  // str << "GET / HTTP/1.1\r\n"
  //   "Host: https://192.168.199.135:8888\r\n"
  //   "\r\n";
  // str.flush();
  // Poco::StreamCopier::copyStream(str, std::cout);

  try{
    URI uri("https://192.168.199.135:8888/api/patient/all");
    HTTPSClientSession s(uri.getHost(), uri.getPort());
    HTTPRequest request(HTTPRequest::HTTP_GET, uri.getPath());
    // request.setKeepAlive(true);
    // request.setContentType("application/json");
    // // Poco::JSON::Object::Ptr reqBody = new Poco::JSON::Object;
    // // reqBody->set("email", "lc@omk.io");
    // // reqBody->set("password", "40BD001563085FC35165329EA1FF5C5ECBDBBEEF");
    // std::string reqBody("{\"email\":\"lc@omk.io\",\"password\":\"40BD001563085FC35165329EA1FF5C5ECBDBBEEF\"}");
    // request.setContentLength( reqBody.length() );

    std::ostream& myOStream = s.sendRequest(request); // sends request, returns open stream
    // myOStream << reqBody;
    //s.sendRequest(request);
    HTTPResponse response;
    std::istream& rs = s.receiveResponse(response);
    StreamCopier::copyStream(rs, std::cout);
  } catch (Exception& ex)
      {
        std::cout << ex.displayText() << std::endl;
        return 1;
      }

  return 0;
}
