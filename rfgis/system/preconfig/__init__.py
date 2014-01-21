print "Preconfiguring"


import qgismobility
import os
import argparse
import sys

def main(configure):
    parser = argparse.ArgumentParser()
    parser.add_argument("--prefix-path", action="store",
                        default=configure.default_prefix_path())
    parser.add_argument("--plugin-path", action="store",
                        default=configure.default_plugin_path())
    parser.add_argument("--load-path", action="store", default="")
    ns = parser.parse_args()
    load_path = os.pathsep.join([os.pathsep.join(sys.path),
                                 os.environ['AUTOCONF_PROJECT_CODE_PATH']])
    
    if ns.load_path != None and ns.load_path != "":
        load_path = os.pathsep.join([ns.load_path, load_path])
        sys.path.append(os.path.abspath(ns.load_path))

    sys.path.append(os.path.abspath(os.environ['AUTOCONF_PROJECT_CODE_PATH']))
    
    idiom_dict = dict(vars(ns)).copy()
    idiom_dict['load_path'] = load_path;
    
    configure.set_preconfiguration_dictionary(idiom_dict)


main(rfgis.QgsMobilityConfigure())

