#include <libxml/tree.h>

#define COLOR_1 "#0000ff"
#define COLOR_2 "#00ffff"
#define COLOR_3 "#8A2BE2"
#define COLOR_4 "#A52A2A"
#define COLOR_5 "#7FFF00"
#define COLOR_6 "#006400"
#define COLOR_7 "#FFD700"
#define COLOR_8 "#FF69B4"
#define COLOR_9 "#FF4500"
#define COLOR_10 "#C0C0C0"

#define XMLNS_CY "'http://www.cytoscape.org'"
#define XMLNS_DC "'http://purl.org/dc/elements/1.1/'"
#define XMLNS_RDF "'http://www.w3.org/1999/02/22-rdf-syntax-ns#'"
#define XMLNS_XLINK "'http://www.w3.org/1999/xlink'"
#define XMLNS "'http://www.cs.rpi.edu/XGMML'"

#define DESIRED_TOP_CLUSTERS 5


// TODO consider moving back to xml_writer or the other one if unused in other places!
int write_all_nodes(network *net, int num_of_clusters, xmlNodePtr *pRoot);
char *color_from_cluster_id(int cluster_id);
int add_xml_edge(xmlNodePtr *pRoot, char *src_name, char *dst_name, int src, int dst, double weight);
int write_all_edges(network *net, int **edges, int num_of_clusters, xmlNodePtr *pRoot);
