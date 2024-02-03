"""

Binary : Expr left, Token op, Expr right

struct Binary : public Expr {
  Binary(std::unique_ptr<Expr> left, std::unique_ptr<Token> op,
         std::unique_ptr<Expr> right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

  std::unique_ptr<Expr> left;
  std::unique_ptr<Token> op;
  std::unique_ptr<Expr> right;
};

"""

from pathlib import Path


from typing import NamedTuple


class _Field(NamedTuple):
    typename: str
    name: str

    @staticmethod
    def parse(field: str):
        typename, name = field.split()
        return _Field(typename.strip(), name.strip())


class _Type(NamedTuple):
    name: str
    fields: list[str]

    @staticmethod
    def parse(line: str):
        name, fields = line.strip().split(":")
        fields = [_Field.parse(f) for f in fields.strip().split(",")]
        return _Type(name.strip(), fields)

    def define(self, write, baseclass: str):
        write(f"struct {self.name} : public {baseclass} {{\n")

        # Constructor
        param_str = ", ".join(
            (f"std::unique_ptr<{f.typename}> {f.name}" for f in self.fields)
        )
        initializer_str = ",".join(
            (f"{f.name}(std::move({f.name}))" for f in self.fields)
        )
        write(f"{self.name}({param_str}) : {initializer_str} {{}}\n")
        write("\n")

        # Visitor accept
        write(
            f"void accept(Visitor &visitor) override {{ visitor.visit{self.name}{baseclass}(*this); }}\n\n"
        )

        # Fields
        member_str = "\n".join(
            (f"std::unique_ptr<{f.typename}> {f.name};" for f in self.fields)
        )
        write(f"{member_str}\n")
        write("};\n\n")


def define_visitor(write, baseclass: str, _all_types: list[_Type]):
    write(f"struct Visitor {{\n")
    for _type in _all_types:
        write(f"    virtual void visit{_type.name}{baseclass}({_type.name} &) = 0;\n")
    write("};\n")


def define_ast(outdir: str, namespace: str, baseclass: str, all_types: list[str]):
    _all_types = [_Type.parse(l) for l in all_types]

    outfile = Path(outdir) / f"{baseclass}.hpp"
    with open(outfile, "w") as fp:
        write = fp.write
        # write = lambda x: print(x, end="")

        # Headers
        write("#pragma once\n\n")
        write("#include <memory>\n\n")
        write('#include "Token.hpp"\n\n')

        # Open namespace
        write(f"namespace {namespace} {{\n\n")

        # Forward declarations
        write("// Forward declarations\n")
        write("\n".join(f"struct {_type.name};" for _type in _all_types))
        write("\n\n")

        # Define baseclass, including the visitor
        write("// Baseclass\n")
        write(f"struct {baseclass} {{\n")
        write(f"\n")
        define_visitor(write, baseclass, _all_types)
        write(f"virtual ~{baseclass}() = default;\n")
        write("virtual void accept(Visitor &visitor) = 0;\n")
        write("};\n\n")

        # Define subclasses
        for _type in _all_types:
            _type.define(write, baseclass)

        # Close namespace
        write("}\n")


baseclass = "Expr"


all_types = """
Binary : Expr left, Token op, Expr right
Grouping : Expr expression
Literal : Object value
Unary : Token op, Expr right
""".strip().split(
    "\n"
)

# line = "Binary : Expr left, Token op, Expr right"
# name, fields = line.strip().split(":")
# define_type(print, baseclass, name.strip(), fields.strip())


define_ast(
    outdir="src",
    namespace="lox::ast",
    baseclass=baseclass,
    all_types=all_types,
)
