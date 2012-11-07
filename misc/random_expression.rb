#!/usr/bin/env ruby

# small (messy) script to generate random expressions

BINOPS = { :add => '+', :sub => '-', :mul => '*', :div => '/' }

$count = 25
$name = "func"
$require_vars = -1
$vars = ("a".."z").to_a.reverse
$used = {}

$size = 1
def next_var use_new=false
  v = nil
  if use_new
    $vars.each do |vv|
      unless $used[vv]
        $vars.delete v
        v = vv
        break
      end
    end
  else
    v = $vars.pop
  end

  unless v
    $size += 1
    $vars.concat ('a'*$size ... 'z'*$size).to_a.reverse
    v = $vars.pop
  end
  $vars.unshift v if rand > 0.4 and not use_new
  $used[v] = true
  v
end

$expr = next_var

if ARGV.length == 0
  puts "No args recieved. using -n[ame]=func, -d[epth] = 25, -p[arams-required] = 0"
end

while arg = ARGV.shift
  if arg[0] == "-"
    case arg[1]
    when 'n' then $name = ARGV.shift
    when 'd' then $count = ARGV.shift.to_i
    when 'p' then $require_vars = ARGV.shift.to_i
    else puts "known flags are -n <func_name>, -d <depth>, -p <required params>"
    end
  end
end

class Neg
  attr_accessor :value
  def initialize a
    @value = a
  end
  def to_s
    "(-#{@value.to_s})"
  end
end

class Bin
  attr_accessor :lhs, :rhs, :type
  def initialize l, r
    @type = BINOPS.keys.sample
    if rand() > 0.5
      @lhs, @rhs = l, r
    else
      @lhs, @rhs = r, l
    end
  end
  def to_s
    "(#{@lhs.to_s} #{BINOPS[@type]} #{@rhs.to_s})"
  end
end

class Expt
  attr_accessor :value, :power
  def initialize v, p
    @value, @power = v, p
  end
  def to_s
    "(#{@value.to_s}^#{@power})"
  end
end


def get_exp from = nil, force_var = false
  if force_var
    if from then
      Bin.new next_var(true), from
    else
      next_var true
    end
  elsif from
    case [:bin_expr, :expt, :neg, :bin_expr, :bin_expr].sample
    when :bin_expr then Bin.new get_exp, from
    when :neg then Neg.new from
    when :expt then Expt.new from, (rand 4)+1
    end
  else
    case [:var, :var, :var, :num, :num].sample
    when :var then next_var
    when :num then (rand 200) - 100
    end
  end
end



$count.times {$expr = get_exp $expr}
if $require_vars > 0
  $expr = get_exp $expr, true while $used.keys.length < $require_vars
end

puts "#{$name}(#{$used.keys.sort.join ', '}) = #{$expr.to_s}"

