#encoding: UTF-8
#【グラフを可視化し、図へ保存するプログラム】2013.01/17,17:29
#ARGV[0] 読み込むファイルの名前
#ARGV[1] 保存するファイルの名前
#ARGV[2] 保存するファイルの拡張子
require "gviz"
require "csv"

header, *data = CSV.read("#{ARGV[0]}")
data = data.map { |id, name, *link| [id.intern, name, link.map(&:intern)] }
 
gv = Gviz.new(:Pref)
 
gv.graph do
  global label:"スケールフリーネットワーク -#{ARGV[0]}-", layout:'neato', overlap:false
  edges arrowhead:'none'
  data.each do |id, name, link|
    route id => link
    node id, label: name, style:'filled'
  end
end

gv.save(:"#{ARGV[1]}", :"#{ARGV[2]}")