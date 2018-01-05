import argparse
import os

HERE = os.path.dirname(os.path.abspath(__file__))


def get_args():
    parser = argparse.ArgumentParser(
        description='Generate a new example project')
    parser.add_argument('--name', metavar='Project name.', type=str,
                        help="Name of the example project to be generated",
                        required=False)
    return parser.parse_args()


def find_template():
    expected = os.path.join(HERE,
                            'examples',
                            'src',
                            '__template')
    if not os.path.isdir(expected):
        print(
            """FATAL: template project not found.
Is the script in the root of the project?
Current directory: {path}""".format(path=HERE))
        raise RuntimeError("Template not found")
    print("Template found\n")
    return os.path.relpath(expected)


def generate_project(name, template_path):
    root_dir = template_path.replace("__template", "")
    dest_dir = os.path.join(root_dir, name)
    if(os.path.isdir(dest_dir)):
        raise RuntimeError(
            "Project directory with the name [%s] already exists in path\n%s\n"
            % (name, dest_dir))
    os.makedirs(dest_dir)
    os.makedirs(os.path.join(HERE, "examples", "assets", name))
    for src_root, dirs, files in os.walk(template_path):
        for f in files:
            src_path = os.path.join(src_root, f)
            dst_path = src_path.replace("__template", name)
            with open(src_path, 'r') as srcfile,\
                    open(dst_path, 'w') as dstfile:
                content = srcfile.read()
                content = content.replace("__template", name)
                dstfile.write(content)


def main():
    try:
        args = get_args()
        template_path = find_template()
        name = args.name if args.name else input("Name: ")
        generate_project(name, template_path)
        print("Project [%s] was generated successfully!" % name)
    except RuntimeError as e:
        print("Unexpected error happened. Check your configuration!",
              e,
              sep='\n')
    except:
        print("Unexpected error")
        raise


if __name__ == '__main__':
    main()
