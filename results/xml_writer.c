#include "common.h"
#include "io.h"
#include "xml.h"

int make_xml_skeleton(xmlDocPtr *pXMLDom, xmlNodePtr *pRoot) {
	*pXMLDom = xmlNewDoc(BAD_CAST "1.0");
	*pRoot   = xmlNewNode(NULL, BAD_CAST "graph");
	xmlDocSetRootElement(*pXMLDom, *pRoot);
	xmlNewProp(*pRoot, BAD_CAST "directed",BAD_CAST "0");
	xmlNewProp(*pRoot, BAD_CAST "xmlns:cy", BAD_CAST XMLNS_CY);
	xmlNewProp(*pRoot, BAD_CAST "xmlns:dc", BAD_CAST XMLNS_DC);
	xmlNewProp(*pRoot, BAD_CAST "xmlns:rdf", BAD_CAST XMLNS_RDF);
	xmlNewProp(*pRoot, BAD_CAST "xmlns:xlink", BAD_CAST XMLNS_XLINK);
	xmlNewProp(*pRoot, BAD_CAST "xmlns", BAD_CAST XMLNS);
	return 0;
}


int best_solution_writer(network *net, int **edges, char *out_dir) {
	char *filename = get_full_path(out_dir, BEST_CLUSTERS);
	if (filename == NULL) {
		return 2;
	}
	return write_all(net, edges, DESIRED_TOP_CLUSTERS, filename);
}

int cluster_sol_writer(network *net, int **edges, char *out_dir, int num_of_clusters) {
	char *filename = get_full_path(out_dir, CLUSTER_SOL);
	if (filename == NULL) {
		return 2;
	}
	return write_all(net, edges, num_of_clusters, filename);
}

int write_all(network *net, int **edges, int num_of_clusters, char *filename) {

	xmlDocPtr	pXMLDom = NULL;
	xmlNodePtr	pRoot = NULL;

	make_xml_skeleton(&pXMLDom, &pRoot);

	if (write_all_nodes(net, num_of_clusters, &pRoot) == 2) {
		return 2;
	}

	if (write_all_edges(net, edges, num_of_clusters, &pRoot) == 2) {
		return 2;
	}

	xmlSaveFormatFileEnc(filename, pXMLDom, "UTF-8");

	xmlFreeDoc(pXMLDom);
	xmlCleanupParser();
	xmlMemoryDump();

	return 0;

}


int write_all_edges(network *net, int **edges, int num_of_clusters, xmlNodePtr *pRoot) {
	vertex **v = (vertex **) net->vertices;

	for (int i = 0; i < net->num_of_vertices; i++) {
		edge **arr_edges = (edge **) v[i]->adjacency_list;
		for (int j = 0; j < v[i]->deg; j++) {
			int other_vertex_id;
			if (arr_edges[j]->from == i) {
				other_vertex_id = arr_edges[j]->to;
			} else {
				other_vertex_id = arr_edges[j]->from;
			}
			if ((i < other_vertex_id) && (edges[i][other_vertex_id - i - 1] == 1)) {
				if ((v[i]->cluster_id < num_of_clusters) && (v[other_vertex_id]->cluster_id < num_of_clusters)) {
					int returned_value = add_edge(pRoot, v[i]->name, v[other_vertex_id]->name, i, other_vertex_id, arr_edges[j]->weight);
					if (returned_value == 2) {
						return 2;
					}
				}
			}
		}
	}

	return 0;
}

int add_edge(xmlNodePtr *pRoot, char *src_name, char *dst_name, int src, int dst, double weight) {
	xmlNodePtr pEdge = xmlNewChild(*pRoot, NULL, BAD_CAST "edge", NULL);
	xmlNodePtr pGraphics = NULL;

	char *interaction = "(interaction detected)";
	char *src_as_str = int_to_string(src);
	char *dst_as_str = int_to_string(dst);

	char *label;
	char cy_edgeLabel[15];

	if ((src_as_str == NULL) || (dst_as_str == NULL)) {
		return 2;
	}

	label = (char *) malloc(strlen(src_name) + strlen(dst_name) + 2 + strlen(interaction) + 1);
	if (label == NULL) {
		send_perror("malloc");
		return 2;
	}

	if (sprintf(label, "%s %s %s", src_name, interaction, dst_name) < 0) {
		send_perror("sprintf");
		return 2;
	}

	xmlNewProp(pEdge, BAD_CAST "label", BAD_CAST label);
	xmlNewProp(pEdge, BAD_CAST "source", BAD_CAST src_as_str);
	xmlNewProp(pEdge, BAD_CAST "target", BAD_CAST dst_as_str);

	pGraphics = xmlNewChild(pEdge, NULL, BAD_CAST "graphics", NULL);

	if (sprintf(cy_edgeLabel, "weight = %1.3lf", weight) < 0) {
		send_perror("sprintf");
		return 2;
	}
	xmlNewProp(pGraphics, BAD_CAST "cy:edgeLabel", BAD_CAST cy_edgeLabel);

	return 0;
}


int write_all_nodes(network *net, int num_of_clusters, xmlNodePtr *pRoot) {
	int i;

	for (i = 0; i < net->num_of_vertices; i++) {
		vertex *v = get_vertex(i, net);
		char *label = v->name;
		int	id = v->id;
		int cluster_id = v->cluster_id;
		if (cluster_id <= num_of_clusters) {
			if (add_node(pRoot, label, id, cluster_id) == 2) {
				return 2;
			}
		}
	}

	return 0;
}

int add_node(xmlNodePtr *pRoot, char *label, int id, int cluster_id) {
	xmlNodePtr pNode = xmlNewChild(*pRoot, NULL, BAD_CAST "node", NULL);
	xmlNodePtr pGraphics;
	char *id_as_str = int_to_string(id);
	if (id_as_str == NULL) {
		return 2;
	}

	xmlNewProp(pNode, BAD_CAST "label", BAD_CAST label);
	xmlNewProp(pNode, BAD_CAST "id", BAD_CAST id_as_str);

	// Add graphics
	pGraphics = xmlNewChild(pNode, NULL, BAD_CAST "graphics", NULL);
	xmlNewProp(pGraphics, BAD_CAST "type", BAD_CAST "ELLIPSE");
	xmlNewProp(pGraphics, BAD_CAST "fill", BAD_CAST color_from_cluster_id(cluster_id));
	xmlNewProp(pGraphics, BAD_CAST "width", BAD_CAST "4");
	xmlNewProp(pGraphics, BAD_CAST "cy:nodeLabel", BAD_CAST label);
	xmlNewProp(pGraphics, BAD_CAST "cy:borderLineType", BAD_CAST "solid");

	return 0;
}

char* int_to_string(int x) {
	// x is an int, so 4 bytes, max size 2^32, 4 billion - that's 10 digits.
	char str[10];
	if (sprintf(str, "%d", x) < 0) {
		send_perror("sprintf");
		return NULL;
	}
	return str;
}

char *color_from_cluster_id(int cluster_id) {
	switch (cluster_id) {
	case 1: return COLOR_1;
	case 2: return COLOR_2;
	case 3: return COLOR_3;
	case 4: return COLOR_4;
	case 5: return COLOR_5;
	case 6: return COLOR_6;
	case 7: return COLOR_7;
	case 8: return COLOR_8;
	case 9: return COLOR_9;
	case 10: return COLOR_10;
	default: return COLOR_10;
	}
}
