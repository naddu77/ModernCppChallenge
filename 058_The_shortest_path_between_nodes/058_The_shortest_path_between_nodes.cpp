// 058_The_shortest_path_between_nodes.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - boost::graph 이용
// - 최단 경로를 구하기 위해 재귀 함수 이용

#include "pch.h"
#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>

using namespace boost;

//A->A : 0     A
//A->B : 7     A->B
//A->C : 9     A->C
//A->D : 20    A->C->D
//A->E : 20    A->C->F->E
//A->F : 11    A->C->F

std::array<char, 6> name = { 'A', 'B', 'C', 'D', 'E', 'F' };
using GraphType = adjacency_list<listS, vecS, directedS, no_property, property<edge_weight_t, int>>;
using VertexDescriptor = graph_traits<GraphType>::vertex_descriptor;
using Edge = std::pair<int, int>;

auto GetParentPath(std::vector<VertexDescriptor> const& p, std::size_t const n) -> std::string
{
	return p[n] != 0 ? GetParentPath(p, p[n]) + " -> " + name[p[n]] : std::string(1, name[p[n]]);
}

int main()
{
	auto const num_nodes = 6;
	enum Nodes { A, B, C, D, E, F };
	std::array<Edge, 18> edge_array = { {
		{ A, B }, { A, C }, { A, F }, { B, C }, { B, D }, { C, D }, { C, F }, { D, E }, { E, F },
		{ B, A }, { C, A }, { F, A }, { C, B }, { D, B }, { D, C }, { F, C }, { E, D }, { F, E }
	} };
	std::array<int, 18> weights = { 7, 9, 14, 10, 15, 11, 2, 6, 9, 7, 9, 14, 10, 15, 11, 2, 6, 9 };
	auto num_arcs = std::size(edge_array);
	GraphType g(std::begin(edge_array), std::end(edge_array), weights.data(), num_nodes);
	auto weightmap = get(edge_weight, g);
	std::vector<VertexDescriptor> p(num_vertices(g));
	std::vector<int> d(num_vertices(g));
	VertexDescriptor s = vertex(A, g);

	dijkstra_shortest_paths(g, s,
		predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
		distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));

	graph_traits<GraphType>::vertex_iterator vi, vend;

	for (auto [vi, vend] = vertices(g); vi != vend; ++vi)
	{
		std::cout << "A -> " << name[*vi] << " : " << d[*vi] << '\t' << GetParentPath(p, *vi);

		if (*vi != 0)
			std::cout << " -> " << name[*vi];

		std::cout << '\n';
	}

	std::cout << std::endl;

	return 0;

}
