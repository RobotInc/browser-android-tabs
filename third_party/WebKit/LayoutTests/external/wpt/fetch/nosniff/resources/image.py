import os.path

def main(request, response):
    type = request.GET.first("type", None)

    if type != None and "svg" in type:
        filename = "green-96x96.svg"
    else:
        filename = "blue96x96.png"

    path = os.path.join(os.path.dirname(__file__), "../../../images", filename)
    body = open(path, "rb").read()

    response.add_required_headers = False
    response.writer.write_status(200)
    response.writer.write_header("x-content-type-options", "nosniff")
    response.writer.write_header("content-length", len(body))
    if(type != None):
        response.writer.write_header("content-type", type)
    response.writer.end_headers()

    response.writer.write(body)
