from http.server import HTTPServer, BaseHTTPRequestHandler
from io import BytesIO

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
      
    def log_message(self,*args, **kwargs):
        pass

    def do_PUT(self):   
        content_length = int(self.headers['Content-Length'])
        body = self.rfile.read(content_length)
        print(content_length) 
        print(body)      
        with open("data.bin",mode="wb") as f:
            f.write(body)  
        self.send_response(200)
        self.end_headers()
        response = BytesIO()
        response.write(bytes('Received '+ str(content_length) +
                                     ' bytes of data','utf-8' ))       
        self.wfile.write(response.getvalue())

httpd = HTTPServer(('', 80), SimpleHTTPRequestHandler)
httpd.serve_forever()