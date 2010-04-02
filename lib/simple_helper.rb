module SimpleHelper
  def partial(tpl, options = {})
    tpl_arr = tpl.to_s.split "/"
    tpl = tpl_arr[0..-2].join("/")+"_"+tpl_arr[-1]
    options.merge!(:layout => false)
    haml(:"#{tpl}", options)
  end
end
