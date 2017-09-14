#include<string>
extern const std::string __context_eval_info;
extern const std::string __context_types_info;
const std::string __context_eval_info = std::string("internal.dump = function(o) {\n"
"    var type = typeof o;\n"
"    if (type == \"undefined\") { return \"undefined\"};\n"
"    if ((type == \"boolean\") || (type == \"number\") || (type == \"string\") || (type == \"symbol\") || (type == \"function\")) { return o.toString();}\n"
"    if (type == \"object\") {\n"
"        if (o === null) {\n"
"            return \"null\";\n"
"        }\n"
"        if (o instanceof Array) {\n"
"            var  i = 0; \n"
"            var  result = [];\n"
"            for(var i = 0; i < o.length; i++)\n"
"            {\n"
"                if (i in o)\n"
"                {\n"
"                    result.push(internal.dump(o[i]));\n"
"                }\n"
"            }\n"
"            return \"[\" + result.join(\", \") + \"]\";\n"
"        } else {\n"
"            if (internal.isNativeObject(o)) {\n"
"                if (o.hasOwnProperty('toString')) {\n"
"                    return o.toString();\n"
"                } \n"
"                var hasPrototypeToString = false;\n"
"                if (typeof o.prototype != \"undefined\") {\n"
"                    hasPrototypeToString = (o.prototype.hasOwnProperty('toString') && o.prototype.toString != Object.toString);\n"
"                }\n"
"                if (hasPrototypeToString) {\n"
"                    return o.toString();\n"
"                }\n"
"                return internal.dumpNativeObject(o);\n"
"            }\n"
"            else\n"
"            {\n"
"                if (o.hasOwnProperty('toString')) {\n"
"                    return o.toString();\n"
"                } else {\n"
"                    var asString = o.toString();\n"
"                    if (asString != \"[object Object]\") {\n"
"                        return asString;\n"
"                    } else {\n"
"                        var result = [];\n"
"                        for(var key in o) { result.push(\"\\\"\" + key + \"\\\"\" + ' : ' + internal.dump(o[key]));}\n"
"                    }\n"
"                }\n"
"                return \"{\" + result.join(\", \") + \"}\";\n"
"            }\n"
"        }\n"
"    }\n"
"    return \"\";\n"
"};\n"
"\n"
"internal.log = function() {\n"
"    for (var i = 0; i < arguments.length; i++) {\n"
"        internal.outputString(internal.dump(arguments[i]));\n"
"    }\n"
"};\n");
const std::string __context_types_info = std::string("var sad = {};\n"
"sad.internal = {};\n"
"sad.util = {};\n"
"\n"
"sad.Point2D = SadPoint2D;\n"
"//sad.Point3D = SadPoint3D;\n"
"//sad.Point2I = SadPoint2I;\n"
"//sad.Point3I = SadPoint3I;\n"
"\n"
"\n"
"sad.internal.plus = function(a, b) { return a + b; };\n"
"sad.internal.minus = function(a, b) { return a - b; };\n"
"sad.internal.multiply = function(a, b) { return a * b; };\n"
"sad.internal.divide = function(a, b) { return a / b; };\n"
"\n"
"\n"
"sad.internal.copyObject = function(dest, src) {\n"
"    for(var key in src) {\n"
"        if (src.hasOwnProperty(key)) {\n"
"            dest[key] = src[key];\n"
"        }\n"
"    }\n"
"}\n"
"\n"
"sad.internal.makeMathFns = function(obj) {\n"
"    obj.plus = function(o) {\n"
"        return this.makeWithFn(sad.internal.plus, o);\n"
"    };\n"
"\n"
"    obj.minus = function(o) {\n"
"        return this.makeWithFn(sad.internal.minus, o);\n"
"    };\n"
"\n"
"    obj.multiply = function(o) {\n"
"        return this.makeWithFn(sad.internal.multiply, o);\n"
"    };\n"
"\n"
"    obj.divide = function(o) {\n"
"        return this.makeWithFn(sad.internal.divide, o);\n"
"    };\n"
"};\n"
"\n"
"//  sad.Point2D bindings\n"
"\n"
"sad.Point2D.prototype.to3d = function() {\n"
"    return new sad.Point3D(this.x, this.y, 0);\n"
"};\n"
"\n"
"sad.Point2D.prototype.to3i = function() {\n"
"    return new sad.Point3I(this.x, this.y, 0);\n"
"};\n"
"\n"
"sad.Point2D.prototype.to2i = function() {\n"
"    return new sad.Point2I(this.x, this.y);\n"
"};\n"
"\n"
"sad.Point2D.prototype.mid = function(o) {\n"
"    return new sad.Point2D((this.x + o.x) / 2, (this.y + o.y) / 2);\n"
"};\n"
"\n"
"\n"
"sad.Point2D.prototype.makeWithFn = function(fn, o) {\n"
"    return new sad.Point2D(fn(this.x, o.x), fn(this.y, o.y));\n"
"};\n"
"\n"
"sad.internal.makeMathFns(sad.Point2D.prototype);\n"
"\n"
"sad.Point2D.prototype.toString = function() {\n"
"    return \"sad::Point2D(\" + this.x + ',' + this.y + ')';\n"
"};\n");
