#!/usr/bin/env ruby
require 'mathn'
require 'optparse'

class Param
  attr_accessor :var, :expt
  def initialize var, expt=1
    @var, @expt = var, expt
  end
  def pow(n=1); @expt += n; end
  def to_s
    if expt == 1
      "#{var}"
    else
      "#{var}^#{expt}"
    end
  end
  def == o
    if o.is_a? String
      o == @var
    elsif o.is_a? Param
      @var == o.var
    else
      false
    end
  end
end

class Monomial
  attr_accessor :mult, :params
  def initialize mult=1.0, params=[]
    @mult, @params = mult, params
  end
  def scale(num); @mult *= num; end
  def add_param(v)
    found = false
    @params.each do |p|
      if p == v
        p.pow v.is_a?(Param) ? v.expt : 1
        found = true
      end
    end
    unless found
      if v.is_a? Param
        params.push v
      else
        params.push Param.new(v)
      end
    end
  end
  def << n
    case n
    when Numeric then scale(n)
    when String, Param then add_param(n)
    end
  end
  def multiplier
    Float((@mult * 100.0).floor) / 100.0
  end
  def to_s
    if @params.size > 0
      exprs = @params.reduce([]) {|m, p| m << p.to_s }
      "#{multiplier}*(#{exprs.join('*')})"
    else
      multiplier.to_s
    end
  end
end

def varnames size
  vs, iter = [], 1
  while size > 0
    vv = (('a' * iter)..('z' * iter)).to_a.take(size)
    size -= vv.size
    vs += vv
    iter += 1
  end
  vs.uniq
end

def exponential rng, max = 3, lam=1.5, min=0
  -Math.log(rng.rand) / lam
end

$settings = {
  :degree => 10,
  :size => 10,
  :nvars => 10,
  :force_degree => false,
  :min_mul => -100.0,
  :max_mul => 100.0,
  :name => "func",
  :seed => Random.new_seed,
  :file => false,
  :silent => false
}

def check_pos n, opts, desc
  if n.nil? or n <= 0
    puts "#{desc.to_s.capitalize} must be a positive integer (got #{n})."
    puts opts
    exit
  end
  n
end

oparser = OptionParser.new do |opts|
  opts.on("-dDEGREE", "--degree DEGREE", Integer, "degree of polynomial")                       {|n| $settings[:degree] = check_pos n, opts, :degree }
  opts.on("-sSIZE", "--size SIZE", Integer, "number of monomials in polynomial")                {|n| $settings[:size] = check_pos n, opts, :size }
  opts.on("-pPARAMS", "--params PARAMS", Integer, "number of parameters taken by the function") {|n| $settings[:nvars] = check_pos n, opts, :nvars }
  opts.on("-xMAX", "--max MAX", Float, "maximum constant multiplier for a monomial")            {|m| $settings[:max_mul] = m }
  opts.on("-nMIN", "--min MIN", Float, "minimum constant multiplier for a monomial")            {|m| $settings[:min_mul] = m }
  opts.on("-f", "--[no-]force-degree", "force every monomial to have the maximum degree")       {|b| $settings[:force_degree] = b }
  opts.on("--name NAME", String, "emit a function with the given name")                         {|s| $settings[:name] = s }
  opts.on("--seed SEED", Integer, "Seed the RNG with SEED before generating")                   {|seed| $settings[:seed] = seed }
  opts.on("-oFILE", "--output FILE", "Write output to FILE and not stdout")                     {|file| $settings[:file] = file }
  opts.on("-s", "--silent", "don't emit a listing of the options used to stdout")               { $settings[:silent] = true }
  opts.on("-h", "--help", "Display this message") { puts opts }
end

begin
  oparser.parse!
rescue OptionParser::InvalidOption, OptionParser::MissingArgument
  puts $!.to_s
  puts oparser
  exit
end

unless $settings[:silent]
  puts "Using options: "
  puts $settings.inspect
end

degree = $settings[:degree]

size = $settings[:size]

nvars = $settings[:nvars]

rng = Random.new $settings[:seed]

p = []

#prob = 1.0 / Float(degree)

names = varnames nvars

force_degree = $settings[:force_degree?]

size.times do
  m = Monomial.new
  m << rng.rand($settings[:min_mul]...$settings[:max_mul])
  deg = degree
  deg = (deg * rng.rand).floor unless force_degree
  deg.times {|i| m << names.sample(:random => rng) }
  p << m
end

puts


file = STDOUT
file = File.new $settings[:file] if $settings[:file]

file.write "#{$settings[:name]}(#{names.join ','}) = "
file.puts(p.join("+"))

STDERR.write "#{$settings[:name]}(#{names.join ','}) = "
STDERR.puts(p.join("+"))


